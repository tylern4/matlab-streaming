cmake_minimum_required(VERSION 3.10)
project(matlab-mex VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_OSX_ARCHITECTURES ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif()

# Turn off building tests for external libraries
set(BUILD_TESTING OFF CACHE BOOL "enable tests functionality" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "enable tests functionality" FORCE)
set(CATCH_BUILD_TESTING OFF CACHE BOOL "enable tests functionality" FORCE)
set(CATCH_INSTALL_DOCS OFF CACHE BOOL "enable tests functionality" FORCE)
set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "enable tests functionality" FORCE)
set(ZMQ_BUILD_TESTS OFF CACHE BOOL "enable tests functionality" FORCE)
set(WITH_PERF_TOOL OFF CACHE BOOL "enable tests functionality" FORCE)

add_subdirectory(external/fmt)
add_subdirectory(external/libzmq)
add_subdirectory(external/cppzmq)
add_subdirectory(external/clipp)

set(LIB_LIST "libzmq cppzmq fmt::fmt")

find_package(Matlab)
if(${Matlab_FOUND})
    matlab_add_mex(NAME steream_data SRC src/test.cpp LINK_TO libzmq cppzmq fmt::fmt)
endif()

file(GLOB PROGRAMS ${PROJECT_SOURCE_DIR}/src/*.cxx)
foreach(prog ${PROGRAMS})
    get_filename_component(progname ${prog} NAME)
    string(REPLACE ".cxx" "" progname ${progname})
    add_executable(${progname} ${prog})
    set_target_properties(${progname} PROPERTIES CXX_STANDARD 17)
    target_link_libraries(${progname} PRIVATE libzmq cppzmq fmt::fmt clipp)
    install(TARGETS ${progname} EXPORT ${PROJECT_NAME}Targets RUNTIME DESTINATION bin)
endforeach(prog ${PROGRAMS})
