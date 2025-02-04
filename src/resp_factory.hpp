#pragma once
#include <boost/beast/http.hpp>
#include <string>

namespace rest {

namespace http = boost::beast::http;

class RespFactory {
   private:
    static http::message_generator create_response(
        const http::request<http::string_body> &req, const std::string &msg,
        http::status status);

   public:
    static http::message_generator ok(
        const http::request<http::string_body> &req, const std::string &msg);

    static http::message_generator not_found(
        const http::request<http::string_body> &req, const std::string &msg);

    static http::message_generator bad_request(
        const http::request<http::string_body> &req, const std::string &msg);
};
}  // namespace rest