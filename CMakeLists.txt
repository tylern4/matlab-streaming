cmake_minimum_required(VERSION 3.10)
project(matlab-mex VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_OSX_ARCHITECTURES ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif()

add_subdirectory(external/fmt)
add_subdirectory(external/libzmq)
add_subdirectory(external/cppzmq)


set(LIB_LIST "libzmq cppzmq fmt::fmt")

find_package(Matlab REQUIRED)

matlab_add_mex(NAME steream_data SRC src/test.cpp LINK_TO libzmq cppzmq fmt::fmt)

file(GLOB PROGRAMS ${PROJECT_SOURCE_DIR}/src/*.cxx)
foreach(prog ${PROGRAMS})
    get_filename_component(progname ${prog} NAME)
    string(REPLACE ".cxx" "" progname ${progname})
    add_executable(${progname} ${prog})
    set_target_properties(${progname} PROPERTIES CXX_STANDARD 17)
    target_link_libraries(${progname} PRIVATE libzmq cppzmq fmt::fmt)
    install(TARGETS ${progname} EXPORT ${PROJECT_NAME}Targets RUNTIME DESTINATION bin)
endforeach(prog ${PROGRAMS})
