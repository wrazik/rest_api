#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <boost/beast/http.hpp>

#include "common_ops.hpp"
#include "save_event.hpp"

namespace rest {
enum class MessageType { SAVE_EVENT, GET_MEAN_PATH, UNKNOWN };

template <class Body, class Allocator>
MessageType get_message_type(
    const http::request<Body, http::basic_fields<Allocator>>& req) {
    std::string path = std::string(req.target());
    if (path.starts_with("/paths/") && path.ends_with("/meanLength") &&
        req.method() == http::verb::get &&
        std::count(path.begin(), path.end(), '/') == 3) {
        return MessageType::GET_MEAN_PATH;
    } else if (path.starts_with("/paths/") &&
               req.method() == http::verb::post &&
               std::count(path.begin(), path.end(), '/') == 2) {
        return MessageType::SAVE_EVENT;
    } else {
        return MessageType::UNKNOWN;
    }
}

namespace beast = boost::beast;
namespace http = boost::beast::http;

template <class Body, class Allocator>
http::message_generator handle_request(
    const std::shared_ptr<Store>& store,
    http::request<Body, http::basic_fields<Allocator>>&& req) {
    switch (get_message_type(req)) {
        case MessageType::SAVE_EVENT:
            return process_save_event(store, std::move(req));
        case MessageType::GET_MEAN_PATH:
            std::cout << "get mean\n";
            break;
        case MessageType::UNKNOWN:
            std::cout << "unknown\n";
            break;
    }
    std::cout << "This is example handle " << std::endl;
    for (const auto& el : *store) {
        std::cout << el.first << " " << el.second << std::endl;
    }
    std::cout << "End of example handle " << std::endl;
    std::string path = std::string(req.target());
    std::string body = std::string(req.body());
    std::string method = std::string(req.method_string());

    std::cout << "Request: \n"
              << "path: " << path << "\n"
              << "body: " << body << "\n"
              << "method: " << method << "\n";

    auto const bad_request = [&req](beast::string_view why) {
        http::response<http::string_body> res{http::status::bad_request,
                                              req.version()};

        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // auto const not_found = [&req](beast::string_view target) {
    //     http::response<http::string_body> res{http::status::not_found,
    //                                           req.version()};
    //     res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    //     res.set(http::field::content_type, "text/html");
    //     res.keep_alive(req.keep_alive());
    //     res.body() =
    //         "The resource '" + std::string(target) + "' was not found.";
    //     res.prepare_payload();
    //     return res;
    // };

    // auto const server_error = [&req](beast::string_view what) {
    //     http::response<http::string_body> res{
    //         http::status::internal_server_error, req.version()};
    //     res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    //     res.set(http::field::content_type, "text/html");
    //     res.keep_alive(req.keep_alive());
    //     res.body() = "An error occurred: '" + std::string(what) + "'";
    //     res.prepare_payload();
    //     return res;
    // };

    return bad_request("Unknown HTTP-method");
}
}  // namespace rest