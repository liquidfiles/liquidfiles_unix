#include "engine.h"
#include "attachment_responce.h"
#include "exceptions.h"
#include "filelinks_responce.h"
#include "messages_responce.h"
#include "message_responce.h"

#include <base/lf_string.h>
#include <io/messenger.h>
#include <io/json.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <vector>

#include <errno.h>

namespace lf {

namespace {

constexpr unsigned s_normal_id_size = 22;

std::string s_data;

size_t data_get(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    if (stream != nullptr) {
        fwrite(ptr, size, nmemb, stream);
    } else {
        s_data += std::string(static_cast<char*>(ptr), nmemb);
    }

    return size * nmemb;
}

int progress_function(void* ptr, double td, double nd, 
        double tu, double nu)
{
    int length = 80;
    double fraction = .0;

    if (td > 0.0) {
        fraction = nd / td;
    } else if (tu > 0) {
        fraction = nu / tu;
    } else {
        return 0;
    }
    int dd = fraction * length;
    io::mout << std::fixed << std::setprecision(0);
    io::mout << std::setw(3) << fraction * 100 << "% [";
    if (dd > 0) {
        io::mout << std::setw(dd) << std::setfill('=') << '=';
    }
    if (dd < length) {
        io::mout << std::setw(length - dd) << std::setfill(' ') << ' ';
    }
    io::mout << "]\r";
    io::mout << io::flush;
    return 0;
}

class curl_header_guard
{
public:
    curl_header_guard(CURL* c)
        : m_slist(nullptr)
    {
        m_slist = curl_slist_append(m_slist, "Content-Type: application/json");
        curl_easy_setopt(c, CURLOPT_HTTPHEADER, m_slist);
    }

    ~curl_header_guard()
    {
        curl_slist_free_all(m_slist);
    }

private:
    curl_slist* m_slist;
};

class curl_form_guard
{
public:
    curl_form_guard(curl_httppost* f)
        : m_formpost{f}
    {
    }

    ~curl_form_guard()
    {
        curl_formfree(m_formpost);
    }

private:
    curl_httppost* m_formpost;
};

class progress_guard
{
public:
    progress_guard(CURL* c , report_level s)
        : m_curl(c) , m_report_level(s)
    {
        if (m_report_level >= report_level::normal) {
            curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, &progress_function);
            curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, false);
        }
    }

    void end()
    {
        if (m_report_level >= report_level::normal) {
            io::mout << io::endl;
            curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, 0);
            curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, true);
        }
    }

    ~progress_guard()
    {
    }

private:
    CURL* m_curl;
    report_level m_report_level;
};

class curl_file_guard
{
public:
    curl_file_guard(CURL* c, FILE* f, report_level s)
        : m_curl(c)
        , m_file(f)
        , m_progress(c,s)
    {
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, m_file);
    }

    ~curl_file_guard()
    {
        fclose(m_file);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, 0);
        m_progress.end();
    }

private:
    CURL* m_curl;
    FILE* m_file;
    progress_guard m_progress;
};

}

void engine::init_curl(std::string key, report_level s, validate_cert v)
{
    if (m_curl != nullptr) {
        curl_easy_cleanup(m_curl);
    }
    m_curl = curl_easy_init();
    if (m_curl == nullptr) {
        throw curl_error("Failed to initialize CURL");
    }
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &data_get);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, 0);
    if (!key.empty()) {
        key += ":x";
        curl_easy_setopt(m_curl, CURLOPT_USERPWD, key.c_str());
    }
    if (v == validate_cert::not_validate) {
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
    }
    if (s == report_level::verbose) {
        curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
    }
}

engine::engine()
    : m_curl(nullptr)
{
}

engine::~engine()
{
    if (m_curl != nullptr) {
        curl_easy_cleanup(m_curl);
        m_curl = nullptr;
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
    for (const auto& f : fs) {
        std::string a = attach_impl(server, f, s);
        attachments.insert(a);
    }
    return send_attachments_impl(server, user, subject, message, attachments, s);
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
    for (const auto& f : fs) {
        attach_impl(server, f, s);
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
    curl_httppost* formpost = nullptr;
    curl_httppost* lastptr = nullptr;
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
    progress_guard pg(m_curl, s);
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, formpost);
    if (s >= report_level::normal) {
        io::mout << "Uploading chunk '" << file << "'." << io::endl;
    }
    std::string r = perform();
    pg.end();
    process_attach_chunk_responce(r, s);
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
    } catch (...) {
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
        auto j = nlohmann::json::parse(r);
        message_responce m;
        m.read(j);
        curl_header_guard hg(m_curl);
        const std::vector<attachment_responce>& a = m.attachments();
        std::vector<attachment_responce>::const_iterator i = a.begin();
        while (i != a.end()) {
            download_impl(i->url(), path, i->filename(), s);
            ++i;
        }
    } catch (...) {
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
    auto j = nlohmann::json::parse(r);
    messages_responce m;
    m.read(r);
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
    nlohmann::json j;
    j["request"]["recipient"] = user;
    j["request"]["subject"] = subject;
    j["request"]["message"] = message;
    j["request"]["send_email"] = true;
    auto data = j.dump();
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= report_level::normal) {
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
    if (m_curl == nullptr) {
        m_curl = curl_easy_init();
    }
    if (m_curl == nullptr) {
        throw curl_error("Failed to initialize CURL");
    }
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &data_get);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, 0);
    if (v == validate_cert::not_validate) {
        curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
    }
    if (s == report_level::verbose) {
        curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
    }
    server += "/login";
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    nlohmann::json j;
    j["user"]["email"] = user;
    j["user"]["password"] = password;
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    auto data = j.dump();
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= report_level::normal) {
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
    if (s >= report_level::normal) {
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
    if (s >= report_level::normal) {
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
        if (s >= report_level::normal) {
            io::mout << "Deleting attachment '" << *i << "'" << io::endl;
        }
        perform();
        if (s >= report_level::normal) {
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
    if (s >= report_level::normal) {
        io::mout << "Deleting attachments of the message." << io::endl;
    }
    perform();
    if (s >= report_level::normal) {
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
    filedrop_attachments_impl(server, key, user, subject, message, rs, s);
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
    curl_httppost* formpost = nullptr;
    curl_httppost* lastptr = nullptr;
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "Filedata",
               CURLFORM_FILE, file.c_str(),
               CURLFORM_END);
    curl_form_guard fg(formpost);
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, formpost);
    progress_guard pg(m_curl, s);
    if (s >= report_level::normal) {
        io::mout << "Uploading file '" << file << "'." << io::endl;
    }
    std::string r = perform();
    pg.end();
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
    nlohmann::json j;
    j["message"]["subject"] = subject;
    j["message"]["message"] = message;
    j["message"]["send_email"] = "true";
    j["message"]["authorization"] = 3;
    j["message"]["recipients"] = std::vector<std::string>{1, user};
    j["message"]["attachments"] = fs;
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    auto data = j.dump();
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= report_level::normal) {
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
    nlohmann::json j;
    j["link"]["attachment"] = id;
    if (!expire.empty()) {
        j["link"]["expires_at"] = expire;
    }
    auto data = j.dump();
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= report_level::normal) {
        io::mout << "Creating filelink" << io::endl;
    }
    return process_create_filelink_responce(perform(), s);
}

void engine::process_attach_chunk_responce(const std::string& r, report_level s) const
{
    if (r.empty() || r == " ") {
        if (s >= report_level::normal) {
            io::mout << "Current chunk uploaded successfully." << io::endl;
        }
        return;
    }
    if (r.size() == s_normal_id_size) {
        if (s >= report_level::normal) {
            io::mout << "All chunks of file uploaded successfully. ID: " << r << io::endl;
        }
        return;
    }
    throw request_error("upload_chunk", r);
}

void engine::process_attach_responce(const std::string& r, report_level s) const
{
    if (r.size() == s_normal_id_size) {
        if (s >= report_level::normal) {
            io::mout << "File uploaded successfully. ID: " << r << io::endl;
        }
        return;
    }
    throw request_error("upload", r);
}

std::string engine::process_send_responce(std::string r,
        report_level s) const
{
    /// @todo workaround for json parser as it does not recognize backslash.
    std::replace(r.begin(), r.end(), '\\', '0');
    auto j = nlohmann::json::parse(r);
    try {
        auto id = j["message"]["id"].get<std::string>();
        if (s >= report_level::normal) {
            io::mout << "Message sent successfully. ID: " << id << io::endl;
        }
        return id;
    } catch (std::exception) {
    }
    throw request_error("send", r);
    return "";
}

template <typename T>
void engine::process_output_responce(const std::string& r,
        report_level s, output_format f) const
{
    auto j = nlohmann::json::parse(r);
    T m;
    m.read(j);
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
    if (s >= report_level::normal) {
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
    if (s >= report_level::normal) {
        io::mout << "Getting messages from the server." << io::endl;
    }
    return perform();
}

void engine::download_impl(const std::string& url,
        const std::string& path,
        std::string name,
        report_level s)
{
    if (s >= report_level::normal) {
        io::mout << "Downloading file '" << name << "'" << io::endl;
    }
    if (!path.empty()) {
        name = path + "/" + name;
    }
    FILE* fp = fopen(name.c_str(), "wb");
    if (fp == nullptr) {
        throw file_error(name, strerror(errno));
    }
    curl_file_guard fg(m_curl, fp, s);
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
    perform();
}

std::string engine::get_filedrop_api_key(const std::string& url, report_level s, validate_cert v)
{
    init_curl("", s, v);
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
    curl_header_guard hg(m_curl);
    if (s >= report_level::verbose) {
        io::mout << "Getting filedrop API key" << io::endl;
    }
    std::string r = perform();
    auto j = nlohmann::json::parse(r);
    if (j.find("errors") != j.end()) {
        auto e = j["errors"].get<std::vector<std::string>>()[0];
        throw request_error("filedrop info", e);
    }
    std::string q = j["filedrop"]["api_key"].get<std::string>();
    if (q.empty()) {
        throw request_error("filedrop info", r);
    }
    if (s >= report_level::normal) {
        io::mout << "Got filedrop API key: " << q << io::endl;
    }
    return q;
}

void engine::filedrop_attachments_impl(std::string server, const std::string& key,
        const std::string& user, const std::string& subject,
        const std::string& message, const strings& fs, report_level s)
{
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    nlohmann::json j;
    j["message"]["from"] = user;
    j["message"]["subject"] = subject;
    j["message"]["message"] = message;
    j["message"]["attachments"] = fs;
    auto data = j.dump();
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= report_level::normal) {
        io::mout << "Sending message to filedrop" << io::endl;
    }
    process_filedrop_responce(perform(), s);
}

std::string engine::process_file_request_responce(const std::string& r, report_level s) const
{
    auto j = nlohmann::json::parse(r);
    std::string q = j["request"]["url"].get<std::string>();
    if (q.empty()) {
        throw request_error("file_request", r);
    }
    if (s >= report_level::normal) {
        io::mout << "Request sent successfully. URL: " << q << io::endl;
    }
    return q;
}

std::string engine::process_get_api_key_responce(const std::string& r, report_level s) const
{
    auto j = nlohmann::json::parse(r);
    try {
        return j["user"]["api_key"].get<std::string>();
    } catch (std::exception) {
        throw request_error("get_api_key", j["errors"][0].get<std::string>());
    }
}

std::string engine::process_create_filelink_responce(const std::string& r, report_level s) const
{
    auto j = nlohmann::json::parse(r);
    auto q = j["link"]["url"].get<std::string>();
    if (s >= report_level::normal) {
        io::mout << "Created filelink sucessfully. URL: " << q << io::endl;
    }
    return q;
}

void engine::process_filedrop_responce(const std::string& r, report_level s) const
{
    auto j = nlohmann::json::parse(r);
    if (j.find("errors") != j.end()) {
        auto e = j["errors"].get<std::vector<std::string>>()[0];
        throw request_error("filedrop", e);
    }
    if (s >= report_level::normal) {
        io::mout << j["message"]["status"].get<std::string>() << io::endl;
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
