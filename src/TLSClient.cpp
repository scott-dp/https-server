//
// Created by scott on 07.02.2025.
//
#include <asio.hpp>
#include <asio/ssl.hpp>
#include <iostream>

#include "../include/TLSClient.h"

using namespace std;

asio::awaitable<void> TLSClient::start() {
    auto executor = co_await asio::this_coro::executor;
    asio::ip::tcp::resolver resolver(executor);

    // Resolve host (DNS-lookup if needed)
    auto resolver_results = co_await resolver.async_resolve("localhost", to_string(3000), asio::use_awaitable);

    asio::ssl::context ssl_context(asio::ssl::context::tlsv13_client);
    ssl_context.set_verify_mode(asio::ssl::verify_none);
    asio::ssl::stream<asio::ip::tcp::socket> socket(executor, ssl_context);
    co_await asio::async_connect(socket.lowest_layer(), resolver_results, asio::use_awaitable);
    cout << "Client: connected" << endl;

    co_await socket.async_handshake(asio::ssl::stream_base::client, asio::use_awaitable);
    cout << "Client: handshake successful" << endl;

    std::string message("hello");
    auto bytes_transferred = co_await asio::async_write(socket, asio::buffer(message + "\r\n"), asio::use_awaitable);
    cout << "Client: sent: " << message << endl;

    std::string buffer;
    bytes_transferred = co_await asio::async_read_until(socket, asio::dynamic_buffer(buffer), "\r\n", asio::use_awaitable);
    message = buffer.substr(0, bytes_transferred - 2); // Strip \r\n at end of buffer
    cout << "Client: received: " << message << endl;

    message = "exit";
    bytes_transferred = co_await asio::async_write(socket, asio::buffer(message + "\r\n"), asio::use_awaitable);
    cout << "Client: sent: " << message << endl;
}
