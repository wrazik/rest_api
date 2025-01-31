cmake_minimum_required(VERSION 3.28)
project(rest_api)
set(CMAKE_CXX_STANDARD 23)

find_package(boost_beast REQUIRED CONFIG)
find_package(boost_asio REQUIRED CONFIG)

set(CPP_SOURCES
        src/rest_api.cpp
        src/server.cpp
        src/server.hpp
        src/listener.cpp
        src/listener.hpp
        src/session.cpp
        src/session.hpp
        src/request_handler.hpp
        src/common_ops.cpp
        src/common_ops.hpp
)

add_executable(rest_api ${CPP_SOURCES})

target_link_libraries(rest_api PRIVATE Boost::asio Boost::beast)

add_custom_target(
        check-format
        COMMAND clang-format --dry-run ${CPP_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "checking format in src..."
)

add_custom_target(
        format
        COMMAND clang-format -i ${CPP_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "checking format in src..."
)