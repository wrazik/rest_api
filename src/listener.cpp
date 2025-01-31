#include "listener.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "common_ops.hpp"
#include "session.hpp"

namespace rest {
namespace net = boost::asio;
using tcp = net::ip::tcp;

Listener::Listener(net::io_context& ioc, tcp::endpoint endpoint,
                   std::shared_ptr<Store> store)
    : m_context(ioc),
      m_acceptor(net::make_strand(ioc)),
      m_store(std::move(store)) {
    beast::error_code ec;

    m_acceptor.open(endpoint.protocol(), ec);
    if (ec) {
        fail(ec, "open");
        return;
    }

    m_acceptor.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        fail(ec, "set_option");
        return;
    }

    m_acceptor.bind(endpoint, ec);
    if (ec) {
        fail(ec, "bind");
        return;
    }

    m_acceptor.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        fail(ec, "listen");
        return;
    }
}

void Listener::run() { do_accept(); }

void Listener::do_accept() {
    m_acceptor.async_accept(
        net::make_strand(m_context),
        beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
}

void Listener::on_accept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        fail(ec, "accept");
        return;
    } else {
        std::make_shared<Session>(std::move(socket), m_store)->run();
    }

    do_accept();
}
}  // namespace rest