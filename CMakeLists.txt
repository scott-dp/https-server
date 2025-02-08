cmake_minimum_required(VERSION 3.28)
project(httpsServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(_WIN32_WINNT=0x0A00)

add_executable(httpsServer main.cpp
        src/TLSServer.cpp
        include/TLSServer.h
)
target_link_libraries(httpsServer PRIVATE ws2_32 mswsock)
