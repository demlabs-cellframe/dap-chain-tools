project(dap-chain-tools C)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)


#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/local/lib)
#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} /opt/local/include)
add_subdirectory(symlinks/libdap)
add_subdirectory(symlinks/libdap-chain)


add_executable(dap_chain_cli "sources/dap_chain_cli.c" )
