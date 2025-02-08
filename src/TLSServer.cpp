//
// Created by scott on 07.02.2025.
//
#include <iostream>
#include <asio.hpp>
#include <asio/ssl.hpp>
#include "../include/TLSServer.h"

using namespace std;

asio::awaitable<void> TLSServer::handle_request(asio::ssl::stream<asio::ip::tcp::socket> socket) {
    try {
        co_await socket.async_handshake(asio::ssl::stream_base::server, asio::use_awaitable);
        cout << "Server: handshake successful" << endl;

        for (;;) {
            std::string buffer;
            auto bytes_transferred = co_await asio::async_read_until(socket, asio::dynamic_buffer(buffer), "\r\n\r\n",
                                                                     asio::use_awaitable);
            auto message = buffer.substr(0, bytes_transferred - 2); // Strip \r\n at end of buffer
            cout << "Server: received: " << message << endl;
            // Close socket when "exit" is retrieved from client
            if (message == "exit") {
                cout << "Server: closing connection" << endl;
                // Connection is closed when socket is destroyed
                co_return;
            } else if (buffer.find("GET / HTTP/1.1") != string::npos) {
                buffer = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "\r\n"
                         "<!DOCTYPE HTML>"
                         "<html>"
                         "  <head>"
                         "      <title>200 OK</title>"
                         "  </head>"
                         "  <body>"
                         "      <h1>Index</h1>"
                         "      <p>Your browser sent a request for the index file</p>"
                         "  </body>"
                         "</html>";
            } else if (buffer.find("GET /page1 HTTP/1.1") != string::npos) {
                buffer ="HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "\r\n"
                        "<!DOCTYPE HTML>"
                        "<html>"
                        "  <head>"
                        "      <title>200 OK</title>"
                        "  </head>"
                        "  <body>"
                        "      <h1>You are now on page 1</h1>"
                        "      <p>Your browser sent a request for page 1<br></p>"
                        "  </body>"
                        "</html>";
            } else if (buffer.find("GET /page2 HTTP/1.1") != string::npos){
                buffer ="HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "\r\n"
                        "<!DOCTYPE HTML>"
                        "<html>"
                        "  <head>"
                        "      <title>200 OK</title>"
                        "  </head>"
                        "  <body>"
                        "      <h1>You are now on page 2</h1>"
                        "      <p>Your browser sent a request for page 2<br></p>"
                        "      <p>Welcome to page 2</p>"
                        "  </body>"
                        "</html>";
            } else {
                buffer ="HTTP/1.1 400 Bad request\r\n"
                        "Content-Type: text/html\r\n"
                        "\r\n"
                        "<!DOCTYPE HTML>"
                        "<html>"
                        "  <head>"
                        "      <title>400 Bad request</title>"
                        "  </head>"
                        "  <body>"
                        "      <h1>400 Bad request baller</h1>"
                        "  </body>"
                        "</html>";
            }
            bytes_transferred = co_await asio::async_write(socket, asio::buffer(buffer), asio::use_awaitable);
            cout << "Server: sent: " << message << endl;
        }
    }catch (const exception& e) {
        cerr << "Server: " << e.what() << endl;
    }
}

asio::awaitable<void> TLSServer::start() {
    try {
        auto executor = co_await asio::this_coro::executor;
        asio::ip::tcp::acceptor acceptor(executor, {asio::ip::tcp::v6(), 3000});

        cout << "Server: waiting for connection" << endl;
        asio::ssl::context ssl_context(asio::ssl::context::tlsv13_server);
        ssl_context.use_certificate_chain_file("cert.crt");
        ssl_context.use_private_key_file("../cert.key", asio::ssl::context::pem);
        for (;;) {
            asio::ssl::stream<asio::ip::tcp::socket> socket(co_await acceptor.async_accept(asio::use_awaitable),
                                                            ssl_context);
            cout << "Server: connection from " << socket.lowest_layer().remote_endpoint().address() << ':'
                 << socket.lowest_layer().remote_endpoint().port() << endl;

            co_spawn(executor, handle_request(std::move(socket)), asio::detached);
        }
    }catch (const exception& e){
        cerr << "Server: " <<e.what() <<endl;
    }
}
