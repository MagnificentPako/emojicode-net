#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <emojicode/runtime/Runtime.h>
#include <emojicode/s/String.h>
#include <httplib.h>
#include <string>
#include <iostream>

// HTTP SERVER
class HServer : public runtime::Object<HServer> {
    public:
        HServer();

        httplib::Server srv;
};

extern "C" void srvGet(HServer *server, s::String *path, runtime::Callable<s::String*> callback) {
    callback.retain();
    server->srv.Get(path->stdString().c_str(), [callback](const httplib::Request& req, httplib::Response& res) {
        s::String *result = callback();
        res.set_content(result->stdString().c_str(), "text/plain");
    });
}

extern "C" void srvStart(HServer *server, s::String *host, runtime::Integer port) {
    server->srv.listen(host->stdString().c_str(), port);
}

// HTTPS CLIENT

class HTTP : public runtime::Object<HTTP> {
    public:
        HTTP(const char* host, int port);
        httplib::Headers convert_headers();

        httplib::SSLClient client;
        std::vector<std::pair<std::string, std::string>> headers;
};


HTTP::HTTP(const char* host, int port) : client(host, port) {}

httplib::Headers HTTP::convert_headers() {
    httplib::Headers hea;

    for(std::vector<std::pair<std::string, std::string>>::iterator it = headers.begin(); it != headers.end(); ++it) {
        hea.insert(*it);
    }

    return hea;
}

extern "C" HTTP* new_client(s::String *host, runtime::Integer port) {
    return HTTP::init(host->stdString().c_str(), port);
}

extern "C" s::String* get(HTTP *http, s::String *path) {
    auto headers = http->convert_headers();
    auto res = http->client.Get(path->stdString().c_str(), headers);

    if(res && res->status == 200) {
        return s::String::init(res->body.c_str());
    } else {
        return s::String::init(" ");
    }
}

extern "C" s::String* post(HTTP *http, s::String *path, s::String *content_type, s::String *data) {
    auto headers = http->convert_headers();
    auto res = http->client.Post(path->stdString().c_str(), headers,
        data->stdString().c_str(),
        content_type->stdString().c_str());
    if(res && res->status == 200) {
        return s::String::init(res->body.c_str());
    } else {
        std::cout << res->status << std::endl;
        std::cout << res->body << std::endl;
        return s::String::init(" ");
    }
}

extern "C" void add_header(HTTP *http, s::String *name, s::String *content) {
    http->headers.push_back(std::make_pair(name->stdString(), 
        content->stdString()));
}

SET_INFO_FOR(HTTP, net, 1f351)
SET_INFO_FOR(HServer, net, 1f454)