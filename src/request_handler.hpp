#pragma once
#include <iostream>
#include <memory>
#include <string>

#include <boost/beast/http.hpp>

#include "common_ops.hpp"

namespace rest {
namespace beast = boost::beast;
namespace http = boost::beast::http;

std::string path_cat(beast::string_view base, beast::string_view path) {
    if (base.empty()) return std::string(path);
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if (result.back() == path_separator) result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for (auto& c : result)
        if (c == '/') c = path_separator;
#else
    char constexpr path_separator = '/';
    if (result.back() == path_separator) result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}

template <class Body, class Allocator>
http::message_generator handle_request(
    const std::shared_ptr<Store>& store,
    http::request<Body, http::basic_fields<Allocator>>&& req) {

    std::cout << "This is example handle " << std::endl;
    //for (const auto& el : *store) {
    //    std::cout << el.first << " " << el.second << std::endl;
    //}
    //std::cout << "End of example handle " << std::endl;
    //std::string path = std::string(req.target());
    //std::string body = std::string(req.body());
    //std::string method = std::string(req.method_string());

    //std::cout << "Request: \n"
    //          << "path: " << path << "\n"
    //          << "body: " << body << "\n"
    //          << "method: " << method << "\n";

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

    //auto const not_found = [&req](beast::string_view target) {
    //    http::response<http::string_body> res{http::status::not_found,
    //                                          req.version()};
    //    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    //    res.set(http::field::content_type, "text/html");
    //    res.keep_alive(req.keep_alive());
    //    res.body() =
    //        "The resource '" + std::string(target) + "' was not found.";
    //    res.prepare_payload();
    //    return res;
    //};

    //auto const server_error = [&req](beast::string_view what) {
    //    http::response<http::string_body> res{
    //        http::status::internal_server_error, req.version()};
    //    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    //    res.set(http::field::content_type, "text/html");
    //    res.keep_alive(req.keep_alive());
    //    res.body() = "An error occurred: '" + std::string(what) + "'";
    //    res.prepare_payload();
    //    return res;
    //};

    return bad_request("Unknown HTTP-method");
}
}  // namespace rest