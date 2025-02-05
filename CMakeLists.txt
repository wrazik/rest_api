cmake_minimum_required(VERSION 3.28)
project(rest_api)
set(CMAKE_CXX_STANDARD 23)

find_package(boost_beast REQUIRED CONFIG)
find_package(boost_asio REQUIRED CONFIG)
find_package(nlohmann_json REQUIRED CONFIG)

if(MSVC)
    set(COMPILE_OPTIONS
            /W4
            /WX
            /permissive-
    )
else()
    set(COMPILE_OPTIONS
            -Wall
            -Wextra
            -Werror
            -Wpedantic
    )
endif()

set(CPP_SOURCES
        src/server.cpp
        src/server.hpp
        src/listener.cpp
        src/listener.hpp
        src/session.cpp
        src/session.hpp
        src/request_handler.hpp
        src/common_ops.cpp
        src/common_ops.hpp
        src/save_event.hpp
        src/store.cpp
        src/store.hpp
        src/get_mean.hpp
        src/resp_factory.cpp
        src/resp_factory.hpp
        src/query.hpp
        src/query.cpp
)


add_library(rest_api_lib ${CPP_SOURCES})
target_link_libraries(rest_api_lib PRIVATE Boost::asio Boost::beast nlohmann_json::nlohmann_json)
target_compile_options(rest_api_lib PRIVATE ${COMPILE_OPTIONS})
target_include_directories(rest_api_lib PUBLIC src)

add_executable(rest_api src/rest_api.cpp)
target_link_libraries(rest_api PRIVATE Boost::asio Boost::beast rest_api_lib)
target_compile_options(rest_api PRIVATE ${COMPILE_OPTIONS})

enable_testing()
set(TEST_SOURCES
        unit_tests/test_save_event.cpp
)
find_package(GTest REQUIRED CONFIG)
include(GoogleTest)
add_executable(run_tests
        ${TEST_SOURCES}
)
target_link_libraries(run_tests PRIVATE GTest::gtest_main rest_api_lib)
target_compile_options(run_tests PRIVATE ${COMPILE_OPTIONS})
add_test(
        NAME run_tests
        COMMAND run_tests)

add_custom_target(
        check-format
        COMMAND clang-format --dry-run ${CPP_SOURCES} ${TEST_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "checking format in src..."
)

add_custom_target(
        format
        COMMAND clang-format -i ${CPP_SOURCES} ${TEST_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "checking format in src..."
)