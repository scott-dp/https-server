//
// Created by scott on 07.02.2025.
//
#include <asio.hpp>
#include <asio/ssl.hpp>

#ifndef HTTPSSERVER_TLSSERVER_H
#define HTTPSSERVER_TLSSERVER_H


class TLSServer {
    asio::awaitable<void> handle_request(asio::ssl::stream<asio::ip::tcp::socket> socket);
public:
    asio::awaitable<void> start();
};


#endif //HTTPSSERVER_TLSSERVER_H
