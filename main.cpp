#include <iostream>
#include <asio.hpp>
#include "include/TLSServer.h"
#include "include/TLSClient.h"

int main() {
    asio::io_context event_loop(1);

    TLSServer echo_server;
    co_spawn(event_loop, echo_server.start(), asio::detached);

    TLSClient echo_client;
    co_spawn(event_loop, echo_client.start(), asio::detached);

    event_loop.run();
}
