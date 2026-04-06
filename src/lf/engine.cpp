#include "engine.h"
#include "attachment_response.h"
#include "exceptions.h"
#include "filelinks_response.h"
#include "messages_response.h"
#include "message_response.h"

#include <base/lf_string.h>
#include <io/messenger.h>
#include <io/json.hpp>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <vector>

#include <errno.h>
#include <sys/stat.h>

namespace lf {

namespace {

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

int progress_function(void* ptr, double td, double nd, double tu, double nu)
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
    curl_header_guard(CURL* c, const std::string& content_type = "application/json")
        : m_slist{nullptr}
    {
        std::string header = "Content-Type: " + content_type;
        m_slist = curl_slist_append(m_slist, header.c_str());
        m_slist = curl_slist_append(m_slist, "Accept: application/json");
        curl_easy_setopt(c, CURLOPT_HTTPHEADER, m_slist);
    }

    ~curl_header_guard()
    {
        curl_slist_free_all(m_slist);
    }

private:
    curl_slist* m_slist;
};

class progress_guard
{
public:
    progress_guard(CURL* c, report_level s)
        : m_curl{c}
        , m_report_level{s}
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
        : m_curl{c}
        , m_file{f}
        , m_progress{c, s}
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

std::string get_basename(const std::string& path)
{
    std::string::size_type i = path.find_last_of('/');
    if (i == std::string::npos) {
        return path;
    }
    return path.substr(i + 1);
}

long get_file_size(const std::string& path)
{
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        return -1;
    }
    return static_cast<long>(st.st_size);
}

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
    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
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
    : m_curl{nullptr}
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
    std::string url = server + "/message/attachments/upload?filename=" +
        filename + "&chunk=" + base::to_string(chunk_id) +
        "&chunks=" + base::to_string(num_chunks);
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());

    FILE* fp = fopen(file.c_str(), "rb");
    if (fp == nullptr) {
        throw file_error(file, strerror(errno));
    }
    long file_size = get_file_size(file);
    curl_header_guard hg(m_curl, "application/octet-stream");
    curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
    curl_easy_setopt(m_curl, CURLOPT_READDATA, fp);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, file_size);

    progress_guard pg(m_curl, s);
    if (s >= report_level::normal) {
        io::mout << "Uploading chunk '" << file << "'." << io::endl;
    }
    std::string r = perform();
    fclose(fp);
    curl_easy_setopt(m_curl, CURLOPT_READDATA, nullptr);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, -1L);
    pg.end();
    process_attach_chunk_response(r, s);
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
    process_output_response<messages_response>(r, s, of);
}

void engine::message(std::string server,
        const std::string& key,
        const std::string& id,
        output_format f,
        report_level s,
        validate_cert v)
{
    std::string r = message_impl(server, key, id, s, v,
        "Getting message from the server.");
    try {
        process_output_response<message_response>(r, s, f);
    } catch (...) {
        throw invalid_message_id(id);
    }
}

namespace {

std::string get_filename_from_url(const std::string& url)
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
        std::string filename = get_filename_from_url(*i);
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
        message_response m;
        m.read(j);
        curl_header_guard hg(m_curl);
        const std::vector<attachment_response>& a = m.attachments();
        std::vector<attachment_response>::const_iterator i = a.begin();
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
    messages_response m;
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
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, -1L);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= report_level::normal) {
        io::mout << "Sending file request to user '" << user << "'" << io::endl;
    }
    return process_file_request_response(perform(), s);
}

std::string engine::get_api_key(std::string server,
        const std::string& user,
        const std::string& password,
        report_level s,
        validate_cert v)
{
    init_curl("", s, v);
    server += "/login";
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    nlohmann::json j;
    j["user"]["email"] = user;
    j["user"]["password"] = password;
    auto data = j.dump();
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, -1L);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= report_level::normal) {
        io::mout << "Getting API key for user '" << user << "'" << io::endl;
    }
    return process_get_api_key_response(perform(), s);
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
    process_output_response<filelinks_response>(perform(), s, of);
}

void engine::delete_attachments(std::string server,
            const std::string& key,
            const std::set<std::string>& ids,
            report_level s,
            validate_cert v)
{
    init_curl(key, s, v);
    server += "/message/attachments/";
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
    curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "DELETE");
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
    for (; i != fs.end(); ++i) {
        std::string id = filedrop_attach_impl(server, *i, s);
        rs.insert(id);
    }
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
    filedrop_attachments_impl(server, key, user, subject, message, fs, s);
}

std::string engine::attach_impl(std::string server,
        const std::string& file,
        report_level s)
{
    std::string filename = get_basename(file);
    std::string url = server + "/message/attachments/upload?filename=" + filename;
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());

    FILE* fp = fopen(file.c_str(), "rb");
    if (fp == nullptr) {
        throw file_error(file, strerror(errno));
    }
    long file_size = get_file_size(file);
    curl_header_guard hg(m_curl, "application/octet-stream");
    curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
    curl_easy_setopt(m_curl, CURLOPT_READDATA, fp);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, file_size);

    progress_guard pg(m_curl, s);
    if (s >= report_level::normal) {
        io::mout << "Uploading file '" << file << "'." << io::endl;
    }
    std::string r = perform();
    fclose(fp);
    curl_easy_setopt(m_curl, CURLOPT_READDATA, nullptr);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, -1L);
    pg.end();
    return process_attach_response(r, s);
}

std::string engine::filedrop_attach_impl(std::string server,
        const std::string& file,
        report_level s)
{
    std::string filename = get_basename(file);
    std::string url = server + "/attachments/upload?filename=" + filename;
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());

    FILE* fp = fopen(file.c_str(), "rb");
    if (fp == nullptr) {
        throw file_error(file, strerror(errno));
    }
    long file_size = get_file_size(file);
    curl_header_guard hg(m_curl, "application/octet-stream");
    curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
    curl_easy_setopt(m_curl, CURLOPT_READDATA, fp);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, file_size);

    progress_guard pg(m_curl, s);
    if (s >= report_level::normal) {
        io::mout << "Uploading file '" << file << "'." << io::endl;
    }
    std::string r = perform();
    fclose(fp);
    curl_easy_setopt(m_curl, CURLOPT_READDATA, nullptr);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, -1L);
    pg.end();
    return process_attach_response(r, s);
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
    j["message"]["send_email"] = true;
    j["message"]["authorization"] = 3;
    j["message"]["recipients"] = std::vector<std::string>{1, user};
    j["message"]["attachments"] = fs;
    curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 0);
    auto data = j.dump();
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, -1L);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= report_level::normal) {
        io::mout << "Sending message to user '" << user << "'" << io::endl;
    }
    return process_send_response(perform(), s);
}

std::string engine::filelink_impl(std::string server,
            const std::string& expire,
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
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, -1L);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= report_level::normal) {
        io::mout << "Creating filelink" << io::endl;
    }
    return process_create_filelink_response(perform(), s);
}

void engine::process_attach_chunk_response(const std::string& r,
        report_level s) const
{
    if (r.empty() || r == " ") {
        if (s >= report_level::normal) {
            io::mout << "Current chunk uploaded successfully." << io::endl;
        }
        return;
    }
    try {
        auto j = nlohmann::json::parse(r);
        if (j.find("attachment") != j.end()) {
            auto id = j["attachment"]["id"].get<std::string>();
            if (s >= report_level::normal) {
                io::mout << "All chunks of file uploaded successfully. ID: "
                    << id << io::endl;
            }
            return;
        }
    } catch (const std::exception&) {
    }
    // Fall back to checking for raw ID string (legacy format)
    if (r.find('{') == std::string::npos && r.size() > 10 && r.size() < 40) {
        if (s >= report_level::normal) {
            io::mout << "All chunks of file uploaded successfully. ID: "
                << r << io::endl;
        }
        return;
    }
    throw request_error("upload_chunk", r);
}

std::string engine::process_attach_response(const std::string& r,
        report_level s) const
{
    try {
        auto j = nlohmann::json::parse(r);
        if (j.find("attachment") != j.end()) {
            auto id = j["attachment"]["id"].get<std::string>();
            if (s >= report_level::normal) {
                io::mout << "File uploaded successfully. ID: " << id << io::endl;
            }
            return id;
        }
    } catch (const std::exception&) {
    }
    // Fall back to checking for raw ID string (legacy format)
    if (r.find('{') == std::string::npos && r.size() > 10 && r.size() < 40) {
        if (s >= report_level::normal) {
            io::mout << "File uploaded successfully. ID: " << r << io::endl;
        }
        return r;
    }
    throw request_error("upload", r);
}

std::string engine::process_send_response(std::string r,
        report_level s) const
{
    // The send response may contain raw newlines in HTML email body,
    // which causes strict JSON parsers to fail. Extract the ID using
    // string search since it appears early in the response.
    std::string id_key = "\"id\":\"";
    auto pos = r.find(id_key);
    if (pos != std::string::npos) {
        pos += id_key.size();
        auto end = r.find('"', pos);
        if (end != std::string::npos) {
            std::string id = r.substr(pos, end - pos);
            if (s >= report_level::normal) {
                io::mout << "Message sent successfully. ID: "
                    << id << io::endl;
            }
            return id;
        }
    }
    throw request_error("send", r.substr(0, 200));
    return "";
}

template <typename T>
void engine::process_output_response(const std::string& r,
        report_level s, output_format f) const
{
    auto j = nlohmann::json::parse(r);
    T m;
    m.read(j);
    io::mout << m.to_string(f);
}

std::string engine::message_impl(std::string server,
        const std::string& key, std::string id,
        report_level s, validate_cert v, std::string log)
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

std::string engine::messages_impl(std::string server,
        const std::string& key, std::string l,
        std::string f, report_level s, validate_cert v)
{
    init_curl(key, s, v);
    server += "/messages/inbox";
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

std::string engine::get_filedrop_api_key(const std::string& url,
        report_level s, validate_cert v)
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

void engine::filedrop_attachments_impl(std::string server,
        const std::string& key, const std::string& user,
        const std::string& subject, const std::string& message,
        const strings& fs, report_level s)
{
    curl_easy_setopt(m_curl, CURLOPT_URL, server.c_str());
    curl_header_guard hg(m_curl);
    nlohmann::json j;
    j["message"]["from"] = user;
    j["message"]["subject"] = subject;
    j["message"]["message"] = message;
    j["message"]["attachments"] = fs;
    auto data = j.dump();
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, -1L);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
    if (s >= report_level::normal) {
        io::mout << "Sending message to filedrop" << io::endl;
    }
    process_filedrop_response(perform(), s);
}

std::string engine::process_file_request_response(const std::string& r,
        report_level s) const
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

std::string engine::process_get_api_key_response(const std::string& r,
        report_level s) const
{
    auto j = nlohmann::json::parse(r);
    if (j.find("errors") != j.end()) {
        throw request_error("get_api_key",
            j["errors"][0].get<std::string>());
    }
    std::string api_key;
    if (j.find("api_key") != j.end() && j["api_key"].is_string()) {
        api_key = j["api_key"].get<std::string>();
    } else if (j.find("user") != j.end()
            && j["user"].find("api_key") != j["user"].end()) {
        api_key = j["user"]["api_key"].get<std::string>();
    } else {
        throw request_error("get_api_key", r);
    }
    if (s >= report_level::normal) {
        io::mout << api_key << io::endl;
    }
    return api_key;
}

std::string engine::process_create_filelink_response(const std::string& r,
        report_level s) const
{
    auto j = nlohmann::json::parse(r);
    auto q = j["link"]["url"].get<std::string>();
    if (s >= report_level::normal) {
        io::mout << "Created filelink successfully. URL: " << q << io::endl;
    }
    return q;
}

void engine::process_filedrop_response(const std::string& r,
        report_level s) const
{
    if (r.empty()) {
        throw request_error("filedrop", "Empty response");
    }
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
    long http_code = 0;
    curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &http_code);
    std::string r = s_data;
    s_data.clear();
    if (http_code >= 400) {
        std::string detail = "Server returned status "
            + base::to_string(http_code);
        try {
            auto j = nlohmann::json::parse(r);
            if (j.find("errors") != j.end() && j["errors"].is_array()
                    && !j["errors"].empty()) {
                detail += ": " + j["errors"][0].get<std::string>();
            } else if (j.find("error") != j.end()) {
                if (j["error"].is_string()) {
                    detail += ": " + j["error"].get<std::string>();
                } else if (j["error"].is_object()) {
                    detail += ": " + j["error"].dump();
                }
            }
        } catch (const std::exception&) {
        }
        throw request_error("HTTP request", detail);
    }
    return r;
}

}
