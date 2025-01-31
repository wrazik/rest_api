#include "session.hpp"

#include <boost/asio/dispatch.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <iostream>

#include "common_ops.hpp"
#include "request_handler.hpp"

namespace rest {

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = net::ip::tcp;

Session::Session(tcp::socket &&socket,
                 std::shared_ptr<std::string const> const &doc_root)
    : stream_(std::move(socket)), doc_root_(doc_root) {}

void Session::run() {
    net::dispatch(
        stream_.get_executor(),
        beast::bind_front_handler(&Session::do_read, shared_from_this()));
}

void Session::do_read() {
    req_ = {};

    stream_.expires_after(std::chrono::seconds(30));

    http::async_read(
        stream_, buffer_, req_,
        beast::bind_front_handler(&Session::on_read, shared_from_this()));
}

void Session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec == http::error::end_of_stream) return do_close();

    if (ec) return fail(ec, "read");

    send_response(handle_request(*doc_root_, std::move(req_)));
}

void Session::send_response(http::message_generator &&msg) {
    bool keep_alive = msg.keep_alive();

    beast::async_write(stream_, std::move(msg),
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
    // Send a TCP shutdown
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

    // At this point the connection is closed gracefully
}
}  // namespace rest