project(dap-chain-tools C)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)


#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/local/lib)
#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} /opt/local/include)
add_subdirectory(ext/libdap/core)
add_subdirectory(ext/libdap/crypt)
add_subdirectory(ext/libdap/http)
add_subdirectory(ext/libdap)
add_subdirectory(ext/libdap-stream/session)
add_subdirectory(ext/libdap-stream/stream)
add_subdirectory(ext/libdap-stream)
add_subdirectory(ext/libdap-chain)


add_executable(dap-chain-cli "sources/dap_chain_cli.c" )
