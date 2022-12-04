cmake_minimum_required(VERSION 3.23)
project(ICMPTunnel C)

set(CMAKE_C_STANDARD 11)

add_executable(ICMPTunnel main.c src/icmp_tunnel.c src/server.c src/server.h src/options.h src/echo_skt.h src/echo_skt.c)
