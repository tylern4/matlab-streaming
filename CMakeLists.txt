cmake_minimum_required(VERSION 3.10)
project(matlab-mex VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_OSX_ARCHITECTURES ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif()

include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
find_package(fmt CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)

find_package(Matlab REQUIRED)

matlab_add_mex(NAME steream_data SRC src/test.cpp LINK_TO libzmq cppzmq cppzmq-static fmt::fmt)


add_executable(client src/client.cxx)
target_link_libraries(client PRIVATE libzmq cppzmq cppzmq-static fmt::fmt)

add_executable(server src/server.cxx)
target_link_libraries(server PRIVATE libzmq cppzmq cppzmq-static fmt::fmt)
