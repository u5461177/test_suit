//
// Created by xtgu on 8/28/20.
//

#ifndef RESTFULTEST_WSSERVER_H
#define RESTFULTEST_WSSERVER_H
#include <map>
#include <chrono>
#include <string>
#include <cstring>
#include <memory>
#include <utility>
#include <cstdlib>
#include <restbed>
#include <system_error>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

namespace ws_server{

    struct Socket_unit{
        std::mutex mtx;
        std::map<std::string, std::shared_ptr<restbed::WebSocket>> sockets;
    };

    class WebSocketServer{

    public:
        WebSocketServer();
        WebSocketServer(const WebSocketServer & webSocketServer) = default;
        WebSocketServer(WebSocketServer && webSocketServer) = default;
        ~WebSocketServer() = default;

    private:
        std::shared_ptr<restbed::Service> _service;
        std::shared_ptr<Socket_unit> _sockets_unit;
    private:
        std::string base64_encode(const unsigned char *input, int length);
        std::multimap<std::string, std::string> build_websocket_handshake_response_headers(const std::shared_ptr<const restbed::Request> &request);
        void ping_handler();
        void close_handler(const std::shared_ptr<restbed::WebSocket> &socket);
        void error_handler(const std::shared_ptr<restbed::WebSocket> &socket, const std::error_code &error);
        void message_handler(const std::shared_ptr<restbed::WebSocket> &source, const std::shared_ptr<restbed::WebSocketMessage> &message);
        void get_method_handler(const std::shared_ptr<restbed::Session> &session);

    public:
        void execute_ws();
        int send_messages(std::string text);
        int send_messages(std::string text, std::string exclude_id);

    };

}

#endif //RESTFULTEST_WSSERVER_H
