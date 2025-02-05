#pragma once

#include <iostream>
#include <vector>

#include <boost/beast/core/string_type.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

#include "common_ops.hpp"
#include "resp_factory.hpp"
#include "store.hpp"

namespace rest {

template <class Body, class Allocator>
http::message_generator process_save_event(
    const std::shared_ptr<Store>& store,
    http::request<Body, http::basic_fields<Allocator>>&& req) {
    std::string event_name = get_event_name(req.target());
    try {
        const auto event = parse_event(req);
        store->save_event(event_name, event);
    } catch (const std::exception& e) {
        return RespFactory::bad_request(req, "Incorrect save event body");
    }
    return RespFactory::ok(req, "OK");
}
}  // namespace rest