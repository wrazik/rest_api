#include "resp_factory.hpp"

namespace rest {
http::message_generator RespFactory::create_response(
    const http::request<http::string_body> &req, const std::string &msg,
    http::status status) {
    http::response<http::string_body> res{status, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());
    res.body() = std::string(msg);
    res.prepare_payload();
    return res;
}

http::message_generator RespFactory::ok(
    const http::request<http::string_body> &req, const std::string &msg) {
    return create_response(req, msg, http::status::ok);
}

http::message_generator RespFactory::not_found(
    const http::request<http::string_body> &req, const std::string &msg) {
    return create_response(req, msg, http::status::not_found);
}

http::message_generator RespFactory::bad_request(
    const http::request<http::string_body> &req, const std::string &msg) {
    return create_response(req, msg, http::status::bad_request);
}
}  // namespace rest