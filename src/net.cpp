#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <emojicode/runtime/Runtime.h>
#include <emojicode/s/String.h>
#include <httplib.h>
#include <string>
#include <iostream>
#include <easywsclient.hpp>
#include <easywsclient.cpp>
#include <assert.h>
#include <regex>
#include <json.hpp>

using json = nlohmann::json;

#include "net.hpp"

// JSON

Json::Json(json j_) {
    j = j_;
}

extern "C" Json* new_json(s::String *input) {
    return Json::init(json::parse(input->stdString().c_str()));
}

extern "C" s::String* netJsonString(Json *js, s::String *name) {
    if(js->j.at(name->stdString()).is_string()) {
        return s::String::init(js->j.at(name->stdString()).get<std::string>().c_str());
    }
    return s::String::init("");
}

extern "C" runtime::Integer netJsonInt(Json *js, s::String *name) {
    if(js->j.at(name->stdString()).is_number_integer()) {
        return js->j.at(name->stdString()).get<runtime::Integer>();
    }
    return 0;
}

extern "C" Json* netJsonObject(Json *js, s::String *name) {
    if(js->j.at(name->stdString()).is_object()) {
        return Json::init(js->j.at(name->stdString()).get<json::object_t>());
    }
    return Json::init(json::object());
}

extern "C" s::String* netJsonDump(Json *js) {
    return s::String::init(js->j.dump().c_str());
}

// REGEX
extern "C" runtime::SimpleOptional<s::String*> netRegex(s::String *str, s::String *pattern, runtime::Integer index) {
    std::regex r(pattern->stdString());
    std::smatch result;
    std::string s(str->stdString());
    std::regex_search(s, result, r);
    if(result[index].str().length()>0) {
        return s::String::init(result[index].str().c_str());
    }
    return runtime::NoValue;
}

// WS CLIENT

WSClient::WSClient(std::string uri_) : uri{uri_} {
    using easywsclient::WebSocket;
    ws = WebSocket::from_url(uri);
    assert(ws);
}

extern "C" WSClient* new_ws_client(s::String *uri) {
    return WSClient::init(uri->stdString());
}

extern "C" void wsclient_run(WSClient *client, runtime::Callable<void, s::String*> callable) {
    using easywsclient::WebSocket;
    callable.retain();
    while(client->ws->getReadyState() != WebSocket::CLOSED) {
        client->ws->poll(1);
        client->ws->dispatch([callable](const std::string & msg) {
            callable(s::String::init(msg.c_str()));
        });
    }
    delete client->ws;
}

extern "C" void wsclient_send(WSClient *client, s::String *msg) {
    client->ws->send(msg->stdString().c_str());
}

// HTTPS CLIENT

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
SET_INFO_FOR(WSClient, net, 1f981) // lion
SET_INFO_FOR(Json, net, 1f335)