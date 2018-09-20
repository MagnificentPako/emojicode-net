#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <emojicode/runtime/Runtime.h>
#include <emojicode/s/String.h>
#include <httplib.h>
#include <cstring>

class HTTP {};

extern "C" s::String* get(s::String* host, s::String* path, runtime::Integer* port) {
    httplib::Client cli(host->cString(), *port);
    auto res = cli.Get(path->cString());
    if(res && res->status == 200) {
        auto str = res->body.c_str();
        return s::String::init(str);
    }
    return s::String::init("");
}

SET_INFO_FOR(HTTP, net, 1f351)