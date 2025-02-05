#pragma once

#include <iostream>
#include <optional>
#include <vector>

#include <boost/beast/core/string_type.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

#include "common_ops.hpp"
#include "query.hpp"
#include "store.hpp"
#include "resp_factory.hpp"

namespace rest {

template <class Body, class Allocator>
http::message_generator get_mean(
    const std::shared_ptr<Store>& store,
    http::request<Body, http::basic_fields<Allocator>>&& req) {
    const auto event_name = get_event_name(req.target());
    try {
        const auto query = [&req]() {
            if (req.body().empty()) {
                return Query{};
            }
            const auto body = nlohmann::json::parse(req.body());
            if (body.empty()) {
                return Query{};
            }
            return body.template get<Query>();
        }();
        const double mean = store->get_mean(event_name, query);
        return RespFactory::ok(req, "{\"mean\": " + std::to_string(mean) + "}");
    } catch (const std::exception& e) {
        return RespFactory::bad_request(req, "Incorrect get mean body");
    }
}
}  // namespace rest
