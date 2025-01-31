#pragma once

#include <iostream>
#include <vector>

#include <boost/beast/core/string_type.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

namespace rest {
std::string get_event_name(boost::beast::string_view path) {
    size_t start = path.find('/', 1) + 1;
    size_t end = path.find('/', start);

    return path.substr(start, end - start);
}

nlohmann::json to_json(const http::request<http::string_body>& req) {
    return nlohmann::json::parse(req.body());
}

std::vector<size_t> get_values(const nlohmann::json& json) {
    return json["values"].get<std::vector<size_t>>();
}

long long int get_date(const nlohmann::json& json) {
    return json["date"].get<long long int>();
}

template <class Body, class Allocator>
http::message_generator process_save_event(
    const std::shared_ptr<Store>& store,
    http::request<Body, http::basic_fields<Allocator>>&& req) {
    std::string event_name = get_event_name(req.target());
    const auto body = to_json(req);
    const auto values = get_values(body);
    for (const auto& value : values) {
        std::cout << "value: " << value << std::endl;
    }
    auto date = get_date(body);
    std::cout << "date: " << date << std::endl;

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
    return bad_request("DEBUG: event_name = " + event_name);
}
}  // namespace rest