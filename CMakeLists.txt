cmake_minimum_required(VERSION 3.10)
project(matlab-mex VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(fmt CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)


find_package(Matlab)
matlab_add_mex(NAME test_mex SRC src/test.cpp LINK_TO libzmq cppzmq cppzmq-static fmt::fmt)

add_executable(client src/client.cxx)
target_link_libraries(client PRIVATE libzmq cppzmq cppzmq-static fmt::fmt)

add_executable(server src/server.cxx)
target_link_libraries(server PRIVATE libzmq cppzmq cppzmq-static fmt::fmt)