#pragma once
#include <map>
#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>

#include "store.hpp"

namespace rest {

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
    beast::tcp_stream m_stream;
    beast::flat_buffer m_buffer;
    std::shared_ptr<Store> m_store;
    http::request<http::string_body> m_req;

   public:
    Session(tcp::socket&& socket, std::shared_ptr<Store> const& store);
    void run();
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void send_response(http::message_generator&& msg);
    void on_write(bool keep_alive, beast::error_code ec,
                  std::size_t bytes_transferred);

    void do_close();
};
}  // namespace rest
