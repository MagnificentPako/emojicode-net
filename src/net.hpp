class WSClient : public runtime::Object<WSClient> {
    public:
        WSClient(std::string uri_);

        std::string uri;
        easywsclient::WebSocket::pointer ws;
};

class HTTP : public runtime::Object<HTTP> {
    public:
        HTTP(const char* host, int port);
        httplib::Headers convert_headers();

        httplib::SSLClient client;
        std::vector<std::pair<std::string, std::string>> headers;
}; 