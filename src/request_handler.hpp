#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <boost/beast/http.hpp>

#include "common_ops.hpp"
#include "resp_factory.hpp"
#include "save_event.hpp"

namespace rest {
enum class MessageType { SAVE_EVENT, GET_MEAN_PATH, HEALTHCHECK, UNKNOWN };

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
    } else if (path == "/healthcheck" && req.method() == http::verb::get) {
        return MessageType::HEALTHCHECK;
    }
    return MessageType::UNKNOWN;
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
        case MessageType::HEALTHCHECK:
            return RespFactory::ok(req, "OK");
        case MessageType::UNKNOWN:
            return RespFactory::not_found(req, "Unknown path");
    }

    return RespFactory::bad_request(req, "Unknown HTTP-method");
}
}  // namespace rest