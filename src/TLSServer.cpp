//
// Created by scott on 07.02.2025.
//
#include <iostream>
#include <asio.hpp>
#include <asio/ssl.hpp>
#include "../include/TLSServer.h"

using namespace std;

asio::awaitable<void> TLSServer::handle_request(asio::ssl::stream<asio::ip::tcp::socket> socket) {
    co_await socket.async_handshake(asio::ssl::stream_base::server, asio::use_awaitable);
    cout << "Server: handshake successful" << endl;

    for (;;) {
        std::string buffer;
        auto bytes_transferred = co_await asio::async_read_until(socket, asio::dynamic_buffer(buffer), "\r\n", asio::use_awaitable);
        auto message = buffer.substr(0, bytes_transferred - 2); // Strip \r\n at end of buffer
        cout << "Server: received: " << message << endl;
        // Close socket when "exit" is retrieved from client
        if (message == "exit") {
            cout << "Server: closing connection" << endl;
            // Connection is closed when socket is destroyed
            co_return;
        }
        bytes_transferred = co_await asio::async_write(socket, asio::buffer(buffer), asio::use_awaitable);
        cout << "Server: sent: " << message << endl;
    }

}

asio::awaitable<void> TLSServer::start() {
    auto executor = co_await asio::this_coro::executor;
    asio::ip::tcp::acceptor acceptor(executor, {asio::ip::tcp::v6(), 3000});

    cout << "Server: waiting for connection" << endl;
    asio::ssl::context ssl_context(asio::ssl::context::tlsv13_server);
    ssl_context.use_certificate_chain_file("../cert.crt");
    ssl_context.use_private_key_file("../cert.key", asio::ssl::context::pem);
    for (;;) {
        asio::ssl::stream<asio::ip::tcp::socket> socket(co_await acceptor.async_accept(asio::use_awaitable), ssl_context);
        cout << "Server: connection from " << socket.lowest_layer().remote_endpoint().address() << ':' << socket.lowest_layer().remote_endpoint().port() << endl;

        co_spawn(executor, handle_request(std::move(socket)), asio::detached);
    }
}
