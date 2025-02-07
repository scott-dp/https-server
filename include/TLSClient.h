//
// Created by scott on 07.02.2025.
//

#ifndef HTTPSSERVER_TLSCLIENT_H
#define HTTPSSERVER_TLSCLIENT_H


#include <asio/awaitable.hpp>

class TLSClient {
public:
    asio::awaitable<void> start();

};


#endif //HTTPSSERVER_TLSCLIENT_H
