cmake_minimum_required(VERSION 3.28)
project(rest_api)
set(CMAKE_CXX_STANDARD 23)

find_package(boost_beast REQUIRED CONFIG)
find_package(boost_asio REQUIRED CONFIG)

add_executable(rest_api rest_api.cpp)

target_link_libraries(rest_api PRIVATE Boost::asio Boost::beast)