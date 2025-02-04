#pragma once

#include <iostream>
#include <vector>

#include <boost/beast/core/string_type.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

#include "common_ops.hpp"
#include "store.hpp"

namespace rest {

template <class Body, class Allocator>
http::message_generator get_mean_path(
    const std::shared_ptr<Store>& store,
    http::request<Body, http::basic_fields<Allocator>>&& req) {
    std::string event_name = get_event_name(req.target());
    try {
        const auto event = parse_event(req);
        store->save_event(event_name, event);
    } catch (const std::exception& e) {
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
        return bad_request("Incorrect save event body");
    }
    http::response<http::empty_body> res{http::status::ok, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.content_length(0);
    return res;
}
}  // namespace rest
