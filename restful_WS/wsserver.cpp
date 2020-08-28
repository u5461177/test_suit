//
// Created by xtgu on 8/28/20.
//
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
#include "wsserver.h"



namespace ws_server{

    using namespace std;
    using namespace restbed;
    using namespace std::chrono;

    WebSocketServer::WebSocketServer():_service(nullptr),_sockets_unit(make_shared<Socket_unit>()) {

    }


    string WebSocketServer::base64_encode(const unsigned char *input, int length) {

        BIO *bmem, *b64;
        BUF_MEM *bptr;
        b64 = BIO_new(BIO_f_base64());
        bmem = BIO_new(BIO_s_mem());
        b64 = BIO_push(b64, bmem);
        BIO_write(b64, input, length);
        (void) BIO_flush(b64);
        BIO_get_mem_ptr(b64, &bptr);

        char *buff = (char *) malloc(bptr->length);
        memcpy(buff, bptr->data, bptr->length - 1);
        buff[bptr->length - 1] = 0;
        BIO_free_all(b64);
        return buff;
    }

    std::multimap<string, string> WebSocketServer::build_websocket_handshake_response_headers(
            const shared_ptr<const restbed::Request> &request) {
        auto key = request->get_header("Sec-WebSocket-Key");
        key.append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
        Byte hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast< const unsigned char * >( key.data()), key.length(), hash);
        multimap<string, string> headers;
        headers.insert(make_pair("Upgrade", "websocket"));
        headers.insert(make_pair("Connection", "Upgrade"));
        headers.insert(make_pair("Sec-WebSocket-Accept", base64_encode(hash, SHA_DIGEST_LENGTH)));
        return headers;
    }


    void WebSocketServer::ping_handler() {
        lock_guard<mutex> guard(_sockets_unit->mtx);
        for (auto &entry : _sockets_unit->sockets) {
            auto key = entry.first;
            auto socket = entry.second;
            if (socket->is_open()) {
                socket->send(WebSocketMessage::PING_FRAME);
            } else {
                socket->close();
            }
        }
    }

    void WebSocketServer::close_handler(const shared_ptr<WebSocket> &socket) {
        if (socket->is_open()) {
            auto response = make_shared<WebSocketMessage>(WebSocketMessage::CONNECTION_CLOSE_FRAME, Bytes({10, 00}));
            socket->send(response);
        }
        const auto key = socket->get_key();
        {
            lock_guard<mutex> guard(_sockets_unit->mtx);
            _sockets_unit->sockets.erase(key);
        }
        fprintf(stderr, "Closed connection to %s.\n", key.data());
    }

    void WebSocketServer::error_handler(const shared_ptr<WebSocket> &socket, const error_code &error) {
        const auto key = socket->get_key();
        fprintf(stderr, "WebSocket Errored '%s' for %s.\n", error.message().data(), key.data());
    }

    void WebSocketServer::message_handler(const shared_ptr<WebSocket> &source, const shared_ptr<WebSocketMessage> &message) {

        const auto opcode = message->get_opcode();

        if (opcode == WebSocketMessage::PING_FRAME) {
            auto response = make_shared<WebSocketMessage>(WebSocketMessage::PONG_FRAME, message->get_data());
            source->send(response);
        } else if (opcode == WebSocketMessage::PONG_FRAME) {
            return;
        } else if (opcode == WebSocketMessage::CONNECTION_CLOSE_FRAME) {
            source->close();
        } else if (opcode == WebSocketMessage::BINARY_FRAME) {
        //We don't support binary data.
        auto response = make_shared<WebSocketMessage>(WebSocketMessage::CONNECTION_CLOSE_FRAME, Bytes({10, 03}));
        source->send(response);
        } else if (opcode == WebSocketMessage::TEXT_FRAME) {

            const auto key = source->get_key();
            const auto data = String::format("%s : %.*s\n", key.data(),  message->get_data().size(),
                                             message->get_data().data());
            fprintf(stderr, "%s", data.data());

            send_messages(data, key);



        }
    }

    void WebSocketServer::get_method_handler(const std::shared_ptr<restbed::Session> &session) {
        const auto request = session->get_request();
        const auto connection_header = request->get_header("connection", String::lowercase);

        if (connection_header.find("upgrade") not_eq string::npos) {
            if (request->get_header("upgrade", String::lowercase) == "websocket") {
                const auto headers = build_websocket_handshake_response_headers(request);

                session->upgrade(SWITCHING_PROTOCOLS, headers, [this](const shared_ptr<WebSocket> socket) {
                    if (socket->is_open()) {
                        socket->set_close_handler([this](const shared_ptr<WebSocket> websocket){
                            close_handler(websocket);
                        });
                        socket->set_error_handler([this](const shared_ptr<WebSocket> websocket, const error_code ec){
                                error_handler(websocket, ec);
                        });

                        socket->set_message_handler([this](const shared_ptr<WebSocket> websocket, const shared_ptr <WebSocketMessage> websocketmsg){
                                message_handler(websocket, websocketmsg);
                        });

                        socket->send("Welcome to Corvusoft Chat!", [this](const shared_ptr<WebSocket> socket) {
                            const auto key = socket->get_key();
                            {
                                lock_guard<mutex> guard(_sockets_unit->mtx);
                                _sockets_unit->sockets.insert(make_pair(key, socket));
                            }

                            fprintf(stderr, "Sent welcome message to %s.\n", key.data());
                        });
                    } else {
                        fprintf(stderr, "WebSocket Negotiation Failed: Client closed connection.\n");
                    }
                });
                return;
            }
        }
        session->close(BAD_REQUEST);
    }

    int WebSocketServer::send_messages(string text) {
        send_messages(text,"");
    }

    int WebSocketServer::send_messages(string text, string exclude_id) {
        auto response = make_shared<WebSocketMessage>(WebSocketMessage::TEXT_FRAME,text);
        lock_guard<mutex> guard(_sockets_unit->mtx);
        for(auto &socket: _sockets_unit->sockets){
            if(socket.second->get_key() != exclude_id) {
                socket.second->send(response);
            }
        }
    }

    void WebSocketServer::execute_ws() {
        auto resource = make_shared<Resource>();
        resource->set_path("/chat");
        resource->set_method_handler("GET", [this](const shared_ptr<Session> session){
                get_method_handler(session);
        });
        auto settings = make_shared<Settings>();
        settings->set_port(1234);
        _service = make_shared<Service>();
        _service->publish(resource);
        _service->schedule([this]{ping_handler();}, milliseconds(5000));
        _service->start(settings);
    }
}

