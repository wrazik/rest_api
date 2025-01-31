#include "session.hpp"

#include <iostream>

#include <boost/asio/dispatch.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>

#include "common_ops.hpp"
#include "request_handler.hpp"

namespace rest {

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = net::ip::tcp;

Session::Session(tcp::socket &&socket, std::shared_ptr<Store> const &store)
    : m_stream(std::move(socket)), m_store(store) {}

void Session::run() {
    net::dispatch(
        m_stream.get_executor(),
        beast::bind_front_handler(&Session::do_read, shared_from_this()));
}

void Session::do_read() {
    m_req = {};

    m_stream.expires_after(std::chrono::seconds(30));

    http::async_read(
        m_stream, m_buffer, m_req,
        beast::bind_front_handler(&Session::on_read, shared_from_this()));
}

void Session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec == http::error::end_of_stream) return do_close();

    if (ec) return fail(ec, "read");

    send_response(handle_request(m_store, std::move(m_req)));
}

void Session::send_response(http::message_generator &&msg) {
    bool keep_alive = msg.keep_alive();

    beast::async_write(m_stream, std::move(msg),
                       beast::bind_front_handler(
                           &Session::on_write, shared_from_this(), keep_alive));
}

void Session::on_write(bool keep_alive, beast::error_code ec,
                       std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) return fail(ec, "write");

    if (!keep_alive) {
        return do_close();
    }

    do_read();
}

void Session::do_close() {
    beast::error_code ec;
    m_stream.socket().shutdown(tcp::socket::shutdown_send, ec);
}
}  // namespace rest