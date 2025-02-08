#include <asio.hpp>
#include "include/TLSServer.h"

int main() {
    asio::io_context event_loop(1);

    TLSServer TLS_server;
    co_spawn(event_loop, TLS_server.start(), asio::detached);

    event_loop.run();
}
