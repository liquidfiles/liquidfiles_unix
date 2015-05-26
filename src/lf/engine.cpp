#include "engine.h"
#include "attachment_responce.h"
#include "exceptions.h"
#include "filelinks_responce.h"
#include "messages_responce.h"
#include "message_responce.h"

#include <base/string.h>
#include <io/messenger.h>
#include <xml/xml.h>
#include <xml/xml_iterators.h>

#include <cstdio>
#include <cstring>
#include <vector>

#include <errno.h>

namespace lf {

namespace {

unsigned s_normal_id_size = 22;

std::string s_data;

size_t data_get(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    s_data += std::string(static_cast<char*>(ptr), nmemb);
    if (stream != 0) {
        fwrite(ptr, size, nmemb, stream);
    }
    return size * nmemb;
}

class curl_header_guard
{
public:
    curl_header_guard(CURL* c)
        : m_slist(0)
    {
        m_slist = curl_slist_append(m_slist, "Content-Type: text/xml");
        curl_easy_setopt(c, CURLOPT_HTTPHEADER, m_slist);
    }

    ~curl_header_guard()
    {
        curl_slist_free_all(m_slist);
    }

private:
    struct curl_slist* m_slist;
};

class curl_form_guard
{
public:
    curl_form_guard(struct curl_httppost* f)
        : m_formpost(f)
    {
    }

    ~curl_form_guard()
    {
        curl_formfree(m_formpost);
    }

private:
    struct curl_httppost* m_formpost;
};

class curl_file_guard
{
public:
    curl_file_guard(CURL* c, FILE* f)
        : m_curl(c)
        , m_file(f)
    {
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, m_file);
    }

    ~curl_file_guard()
    {
        fclose(m_file);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, 0);
    }

private:
    CURL* m_curl;
    FILE* m_file;
};

}

void engine::init_curl(std::string key, report_level s, validate_cert v)
{
    if (m_curl != 0) {
        curl_easy_cleanup(m_curl);
    }
    m_curl = curl_easy_init();
    if (m_curl == 0) {
        throw curl_error("Failed to initialize CURL");
    }
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &data_get);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, 0);
    if (!key.empty()) {
        key += ":x";
        curl_easy_setopt(m_curl, CURLOPT_USERPWD, key.c_str());
    }
    if (v == NOT_VALIDATE) {
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
    }
    if (s == VERBOSE) {
        curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
    }
}

engine::engine()
    : m_curl(0)
{
}

engine::~engine()
{
    if (m_curl != 0) {
        curl_easy_cleanup(m_curl);
        m_curl = 0;
    }
}

std::string engine::send(std::string server,
        const std::string& key,
        const std::string& user,
        const std::string& subject,
        const std::string& message,
        const strings& fs,
        report_level s,
        validate_cert v)
{
    init_curl(key, s, v);
    std::set<std::string> attachments;
    strings::const_iterator i = fs.begin();
    for (; i != fs.end(); ++i) {
        std::string a = attach_impl(server, *i, s);
        attachments.insert(a);
    }
    return send_attachments_impl(server, user, subject, message,
            attachments, s);
}

std::string engine::send_attachments(std::string server,
            const std::string& key,
            const std::string& user,
            const std::string& subject,
            const std::string& message,
            const strings& fs,
            report_level s,
            validate_cert v)
{
    init_curl(key, s, v);
    return send_attachments_impl(server, user, subject, message, fs, s);
}

void engine::attach(std::string server,
        const std::string& key,
        const strings& fs,
        report_level s,
        validate_cert v)
{
    init_curl(key, s, v);
    strings::const_iterator i = fs.begin();
    for (; i != fs.end(); ++i) {
        attach_impl(server, *i, s);
    }
}

void engine::attach(std::string server,
        const std::string& key,
        const std::string& file,
        const std::string& filename,
        int chunk_id,
        int num_chunks,
        report_level s,
        validate_cert v)
{
    init_curl(key, s, v);
    server += "/attachments";
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* lastptr = NULL;
    curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, "Filedata",
            CURLFORM_FILE, file.c_str(),
            CURLFORM_END);
    curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, "name",
            CURLFORM_COPYCONTENTS, filename.c_str(),
            CURLFORM_END);
    curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, "chunk",
            CURLFORM_COPYCONTENTS, base::to_string(chunk_id).c_str(),
            CURLFORM_END);
    curl_formadd(&formpost,
            &lastptr,
            CURLFORM_COPYNAME, "chunks",
            CURLFORM_COPYCONTENTS, base::to_string(num_chunks).c_str(),
            CURLFORM_END);
    curl_form_guard fg(formpost);
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, formpost);
    if (s >= NORMAL) {
        io::mout << "Uploading chunk '" << file << "'." << io::endl;
    }
    process_attach_chunk_responce(perform(), s);
}

void engine::messages(std::string server,
        const std::string& key,
        const std::string& l,
        const std::string& f,
        output_format of,
        report_level s,
        validate_cert v)
{
    std::string r = messages_impl(server, key, l, f, s, v);
    process_output_responce<messages_responce>(r, s, of);
}

void engine::message(std::string server,
        const std::string& key,
        const std::string& id,
        output_format f,
        report_level s,
        validate_cert v)
{
    std::string r =  message_impl(server, key, id, s, v,
            "Getting message from the server.");
    try {
        process_output_responce<message_responce>(r, s, f);
    } catch (xml::parse_error&) {
        throw invalid_message_id(id);
    }
}

namespace {

std::string get_filename(const std::string& url)
{
    std::string::size_type i = url.find_last_of('/');
    if (i == std::string::npos) {
        throw invalid_url(url);
    }
    return url.substr(i + 1);
}

}

void engine::download(const std::set<std::string>& urls,
        const std::string& key,
        const std::string& path,
        report_level s,
        validate_cert v)
{
    init_curl(key, s, v);
    std::set<std::string>::const_iterator i = urls.begin();
    curl_header_guard hg(m_curl);
    while (i != urls.end()) {
        std::string filename = get_filename(*i);
        download_impl(*i, path, filename, s);
        ++i;
    }
}

void engine::download(std::string server,
        const std::string& key,
        const std::string& path,
        const std::string& id,
        report_level s,
        validate_cert v)
{
    std::string r = message_impl(server, key, id, s, v,
        "Retrieving attachments of message.");
    try {
        xml::document<> d;
        d.parse<xml::parse_fastest | xml::parse_no_utf8>(const_cast<char*>(r.c_str()));
        message_responce m;
        m.read(&d);
        curl_header_guard hg(m_curl);
        const std::vector<attachment_responce>& a = m.attachments();
        std::vector<attachment_responce>::const_iterator i = a.begin();
        while (i != a.end()) {
            download_impl(i->url(), path, i->filename(), s);
            ++i;
        }
    } catch (xml::parse_error&) {
        throw invalid_message_id(id);
    }
}

void engine::download(std::string server,
        const std::string& key,
        const std::string& path,
        const std::string& l,
        const std::string& f,
        report_level s,
        validate_cert v)
{
    std::string r = messages_impl(server, key, l, f, s, v);
    xml::document<> d;
    d.parse<xml::parse_fastest | xml::parse_no_utf8>(const_cast<char*>(r.c_str()));
    messages_responce m;
    m.read(&d);
    for (unsigned i = 0; i < m.size(); ++i) {
        download(server, path, key, m.id(i), s, v);
    }
}

std::string engine::file_request(std::string server,
        const std::string& key,
        const std::string& user,
        const std::string& subject,
        const std::string& message,
        report_level s,
        validate_cert v)
{
    init_curl(key, s, v);
    server += "/requests";
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    std::string data = std::string(
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
  <request>\
    <recipient>") + user + std::string("</recipient>\
    <subject>") + subject + std::string("</subject>\
    <message>") + message + std::string("</message>\
    <send_email>true</send_email>\
");
    data += "</request>\n";
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= NORMAL) {
        io::mout << "Sending file request to user '" << user << "'" << io::endl;
    }
    return process_file_request_responce(perform(), s);
}

std::string engine::get_api_key(std::string server,
        const std::string& user,
        const std::string& password,
        report_level s,
        validate_cert v)
{
    if (m_curl == 0) {
        m_curl = curl_easy_init();
    }
    if (m_curl == 0) {
        throw curl_error("Failed to initialize CURL");
    }
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &data_get);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, 0);
    if (v == NOT_VALIDATE) {
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
    }
    if (s == VERBOSE) {
        curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
    }
    server += "/login";
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    std::string data = std::string(
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
  <user>\
    <email>") + user + std::string("</email>\
    <password>") + password + std::string("</password>\
");
    data += "</user>\n";
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= NORMAL) {
        io::mout << "Getting API key for user '" << user << "'" << io::endl;
    }
    return process_get_api_key_responce(perform(), s);
}

std::string engine::filelink(std::string server,
            const std::string& key,
            const std::string& expire,
            const std::string& file,
            report_level s,
            validate_cert v)
{
    init_curl(key, s, v);
    std::string a = attach_impl(server, file, s);
    return filelink_impl(server, expire, a, s);
}

std::string engine::filelink_attachment(std::string server,
            const std::string& key,
            const std::string& expire,
            const std::string& id,
            report_level s,
            validate_cert v)
{
    init_curl(key, s, v);
    return filelink_impl(server, expire, id, s);
}

void engine::delete_filelink(std::string server,
            const std::string& key,
            const std::string& id,
            report_level s,
            validate_cert v)
{
    init_curl(key, s, v);
    server += "/link/";
    server += id;
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    if (s >= NORMAL) {
        io::mout << "Deleting filelink with id '" << id << "'" << io::endl;
    }
    std::string r = perform();
    if (r.find_first_not_of(' ') != r.npos) {
        throw request_error("delete_filelink", r);
    }
    io::mout << "Filelink deleted successfully." << io::endl;
}

void engine::filelinks(std::string server,
            const std::string& key,
            const std::string& limit,
            output_format of,
            report_level s,
            validate_cert v)
{
    init_curl(key, s, v);
    server += "/link";
    if (!limit.empty()) {
        server += "?limit=";
        server += limit;
    }
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    if (s >= NORMAL) {
        io::mout << "Getting filelinks from the server." << io::endl;
    }
    process_output_responce<filelinks_responce>(perform(), s, of);
}

void engine::delete_attachments(std::string server,
            const std::string& key,
            const std::set<std::string>& ids,
            report_level s,
            validate_cert v)
{
    init_curl(key, s, v);
    server += "/attachment/";
    curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_header_guard hg(m_curl);
    std::set<std::string>::const_iterator i = ids.begin();
    for (; i != ids.end(); ++i) {
        std::string x = server + (*i);
        curl_easy_setopt(m_curl, CURLOPT_URL, x.c_str());
        if (s >= NORMAL) {
            io::mout << "Deleting attachment '" << *i << "'" << io::endl;
        }
        perform();
        if (s >= NORMAL) {
            io::mout << "Deleted successfully." << io::endl;
        }
    }
}

void engine::delete_attachments(std::string server,
            const std::string& key,
            const std::string& id,
            report_level s,
            validate_cert v)
{
    init_curl(key, s, v);
    server += "/message/";
    server += id;
    server += "/delete_attachments";
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    if (s >= NORMAL) {
        io::mout << "Deleting attachments of the message." << io::endl;
    }
    perform();
    if (s >= NORMAL) {
        io::mout << "Deleted attachments successfully." << io::endl;
    }
}

namespace {

std::string get_server_from_filedrop(const std::string& server)
{
    std::size_t i = server.find('/', 8);
    return std::string(server, 0, i);
}

}

void engine::filedrop(std::string server,
        const std::string& user,
        const std::string& subject,
        const std::string& message,
        const strings& fs,
        report_level s,
        validate_cert v)
{
    std::string key = get_filedrop_api_key(server, s, v);
    init_curl(key, s, v);
    strings::const_iterator i = fs.begin();
    strings rs;
    std::string url = get_server_from_filedrop(server);
    for (; i != fs.end(); ++i) {
        std::string id = attach_impl(url, *i, s);
        rs.insert(id);
    }
    curl_easy_setopt(m_curl, CURLOPT_USERPWD, "");
    filedrop_attachments_impl(server, key, user, subject, message,
            rs, s);
}

void engine::filedrop_attachments(std::string server,
            const std::string& user,
            const std::string& subject,
            const std::string& message,
            const strings& fs,
            report_level s,
            validate_cert v)
{
    std::string key = get_filedrop_api_key(server, s, v);
    filedrop_attachments_impl(server, key, user, subject, message,
            fs, s);
}

std::string engine::attach_impl(std::string server,
        const std::string& file,
        report_level s)
{
    server += "/attachments";
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* lastptr = NULL;
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "Filedata",
               CURLFORM_FILE, file.c_str(),
               CURLFORM_END);
    curl_form_guard fg(formpost);
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, formpost);
    if (s >= NORMAL) {
        io::mout << "Uploading file '" << file << "'." << io::endl;
    }
    std::string r = perform();
    process_attach_responce(r, s);
    return r;
}

std::string engine::send_attachments_impl(std::string server,
        const std::string& user,
        const std::string& subject,
        const std::string& message,
        const strings& fs,
        report_level s)
{
    server += "/message";
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    std::string data = std::string(
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
  <message>\
    <recipients type=\"array\">\
      <recipient>") + user + std::string("</recipient>\
    </recipients>\
    <subject>") + subject + std::string("</subject>\
    <message>") + message + std::string("</message>\
    <send_email>true</send_email>\
    <authorization>3</authorization>\
    <attachments type='array'>\
");
    for (strings::const_iterator i = fs.begin(); i != fs.end(); ++i) {
        data += "      <attachment>";
        data += *i;
        data += "</attachment>\n";
    }
    data += "    </attachments>\
  </message>\n";
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= NORMAL) {
        io::mout << "Sending message to user '" << user << "'" << io::endl;
    }
    return process_send_responce(perform(), s);
}

std::string engine::filelink_impl(std::string server, const std::string& expire,
            const std::string& id, report_level s)
{
    server += "/link";
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    std::string data = std::string(
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
  <link>\
    <attachment>") + id + "</attachment>\n";
    if (!expire.empty()) {
        data += std::string("<expires_at>") + expire + "</expires_at>\n";
    }
    data += "  </link>\n";
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= NORMAL) {
        io::mout << "Creating filelink" << io::endl;
    }
    return process_create_filelink_responce(perform(), s);
}

void engine::process_attach_chunk_responce(const std::string& r, report_level s) const
{
    if (r.empty() || r == " ") {
        if (s >= NORMAL) {
            io::mout << "Current chunk uploaded successfully." << io::endl;
        }
        return;
    }
    if (r.size() == s_normal_id_size) {
        if (s >= NORMAL) {
            io::mout << "All chunks of file uploaded successfully. ID: " << r << io::endl;
        }
        return;
    }
    throw request_error("upload_chunk", r);
}

void engine::process_attach_responce(const std::string& r, report_level s) const
{
    if (r.size() == s_normal_id_size) {
        if (s >= NORMAL) {
            io::mout << "File uploaded successfully. ID: " << r << io::endl;
        }
        return;
    }
    throw request_error("upload", r);
}

std::string engine::process_send_responce(const std::string& r,
        report_level s) const
{
    xml::document<> d;
    d.parse<xml::parse_fastest | xml::parse_no_utf8>(const_cast<char*>(r.c_str()));
    xml::node_iterator<> i(d.first_node());
    xml::node_iterator<> e;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        if (n == "id") {
            std::string v(i->value(), i->value_size());
            if (s >= NORMAL) {
                io::mout << "Message sent successfully. ID: " << v << io::endl;
            }
            return v;
        }
        ++i;
    }
    throw request_error("send", r);
    return "";
}

template <typename T>
void engine::process_output_responce(const std::string& r,
        report_level s, output_format f) const
{
    xml::document<> d;
    d.parse<xml::parse_fastest | xml::parse_no_utf8>(const_cast<char*>(r.c_str()));
    T m;
    m.read(&d);
    io::mout << m.to_string(f);
}

std::string engine::message_impl(std::string server, const std::string& key,
        std::string id, report_level s, validate_cert v, std::string log)
{
    init_curl(key, s, v);
    server += "/message/";
    server += id;
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    if (s >= NORMAL) {
        io::mout << log << io::endl;
    }
    return perform();
}

std::string engine::messages_impl(std::string server, const std::string& key, std::string l,
        std::string f, report_level s, validate_cert v)
{
    init_curl(key, s, v);
    server += "/message";
    if (!l.empty()) {
        server += "?sent_in_the_last=";
        server += l;
    } else if (!f.empty()) {
        server += "?sent_after=";
        server += f;
    }
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    if (s >= NORMAL) {
        io::mout << "Getting messages from the server." << io::endl;
    }
    return perform();
}

void engine::download_impl(const std::string& url,
        const std::string& path,
        std::string name,
        report_level s)
{
    if (s >= NORMAL) {
        io::mout << "Downloading file '" << name << "'" << io::endl;
    }
    if (!path.empty()) {
        name = path + "/" + name;
    }
    FILE* fp = fopen(name.c_str(), "wb");
    if (fp == 0) {
        throw file_error(name, strerror(errno));
    }
    curl_file_guard fg(m_curl, fp);
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
    perform();
}

std::string engine::get_filedrop_api_key(const std::string& url, report_level s, validate_cert v)
{
    init_curl("", s, v);
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
    curl_header_guard hg(m_curl);
    if (s >= VERBOSE) {
        io::mout << "Getting filedrop API key" << io::endl;
    }
    std::string r = perform();
    xml::document<> d;
    d.parse<xml::parse_fastest | xml::parse_no_utf8>(const_cast<char*>(r.c_str()));
    if (d.first_node() == 0) {
        throw request_error("filedrop info", r);
    }
    std::string h(d.first_node()->name(), d.first_node()->name_size());
    xml::node_iterator<> i(d.first_node());
    xml::node_iterator<> e;
    if (h == "error") {
        while(i != e) {
            std::string n(i->name(), i->name_size());
            if (n == "message") {
                std::string m = std::string(i->value(), i->value_size());
                throw request_error("filedrop info", m);
                break;
            }
            ++i;
        }
        throw request_error("filedrop info", r);
    }
    std::string q;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        if (n == "api_key") {
            q = std::string(i->value(), i->value_size());
            if (s >= VERBOSE) {
                io::mout << "Got filedrop API key: " << q << io::endl;
            }
            break;
        }
        ++i;
    }
    if (q.empty()) {
        throw request_error("filedrop info", r);
    }
    return q;
}

void engine::filedrop_attachments_impl(std::string server, const std::string& key,
        const std::string& user, const std::string& subject,
        const std::string& message, const strings& fs, report_level s)
{
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    std::string data = std::string(
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
  <message>\
    <api_key>") + key + std::string("</api_key>\
    <from>") + user + std::string("</from>\
    <subject>") + subject + std::string("</subject>\
    <message>") + message + std::string("</message>\
    <attachments type='array'>\
");
    for (strings::const_iterator i = fs.begin(); i != fs.end(); ++i) {
        data += "      <attachment>";
        data += *i;
        data += "</attachment>\n";
    }
    data += "    </attachments>\
  </message>\n";
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= NORMAL) {
        io::mout << "Sending message to filedrop" << io::endl;
    }
    process_filedrop_responce(perform(), s);
}

std::string engine::process_file_request_responce(const std::string& r, report_level s) const
{
    xml::document<> d;
    d.parse<xml::parse_fastest | xml::parse_no_utf8>(const_cast<char*>(r.c_str()));

    xml::node_iterator<> i(d.first_node());
    xml::node_iterator<> e;
    std::string q;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        if (n == "url") {
            q = std::string(i->value(), i->value_size());
            if (s >= NORMAL) {
                    io::mout << "Request sent successfully. URL: " << q << io::endl;
            }
            break;
        }
        ++i;
    }
    if (q.empty()) {
        throw request_error("file_request", r);
    }
    return q;
}

std::string engine::process_get_api_key_responce(const std::string& r, report_level s) const
{
    xml::document<> d;
    d.parse<xml::parse_fastest | xml::parse_no_utf8>(const_cast<char*>(r.c_str()));
    if (d.first_node() == 0) {
        throw request_error("get_api_key", r);
    }
    std::string h(d.first_node()->name(), d.first_node()->name_size());
    xml::node_iterator<> i(d.first_node());
    xml::node_iterator<> e;
    if (h == "error") {
        while(i != e) {
            std::string n(i->name(), i->name_size());
            if (n == "message") {
                std::string m = std::string(i->value(), i->value_size());
                throw request_error("get_api_key", m);
                break;
            }
            ++i;
        }
        throw request_error("get_api_key", r);
    }
    std::string q;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        if (n == "api_key") {
            q = std::string(i->value(), i->value_size());
            if (s >= NORMAL) {
                io::mout << "Retrieved API key: " << q << io::endl;
            }
            break;
        }
        ++i;
    }
    if (q.empty()) {
        throw request_error("get_api_key", r);
    }
    return q;
}

std::string engine::process_create_filelink_responce(const std::string& r, report_level s) const
{
    xml::document<> d;
    d.parse<xml::parse_fastest | xml::parse_no_utf8>(const_cast<char*>(r.c_str()));
    if (d.first_node() == 0) {
        throw request_error("create_filelink", r);
    }
    std::string h(d.first_node()->name(), d.first_node()->name_size());
    xml::node_iterator<> i(d.first_node());
    xml::node_iterator<> e;
    std::string q;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        if (n == "url") {
            q = std::string(i->value(), i->value_size());
            if (s >= NORMAL) {
                io::mout << "Created filelink sucessfully. URL: " << q << io::endl;
            }
            break;
        }
        ++i;
    }
    if (q.empty()) {
        throw request_error("create_filelink", r);
    }
    return q;
}

void engine::process_filedrop_responce(const std::string& r, report_level s) const
{
    xml::document<> d;
    d.parse<xml::parse_fastest | xml::parse_no_utf8>(const_cast<char*>(r.c_str()));
    if (d.first_node() == 0) {
        throw request_error("filedrop", r);
    }
    std::string h(d.first_node()->name(), d.first_node()->name_size());
    xml::node_iterator<> i(d.first_node());
    xml::node_iterator<> e;
    std::string q;
    while(i != e) {
        std::string n(i->name(), i->name_size());
        if (n == "status") {
            q = std::string(i->value(), i->value_size());
            if (s >= NORMAL) {
                io::mout << q << io::endl;
            }
            break;
        }
        ++i;
    }
    if (q.empty()) {
        throw request_error("filedrop", r);
    }
}

std::string engine::perform()
{
    CURLcode res = curl_easy_perform(m_curl);
    if (res != CURLE_OK) {
        throw curl_error(std::string(curl_easy_strerror(res)));
    }
    std::string r = s_data;
    s_data.clear();
    return r;
}

}
