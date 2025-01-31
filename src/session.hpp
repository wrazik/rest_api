#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <memory>

namespace rest {

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    std::shared_ptr<std::string const> doc_root_;
    http::request<http::string_body> req_;

   public:
    Session(tcp::socket&& socket,
            std::shared_ptr<std::string const> const& doc_root);
    void run();
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void send_response(http::message_generator&& msg);
    void on_write(bool keep_alive, beast::error_code ec,
                  std::size_t bytes_transferred);

    void do_close();
};
}  // namespace rest
