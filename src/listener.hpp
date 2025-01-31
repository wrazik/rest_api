#pragma once
#include <map>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>

#include "common_types.hpp"

namespace rest {

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Listener : public std::enable_shared_from_this<Listener> {
    net::io_context& m_context;
    tcp::acceptor m_acceptor;
    std::shared_ptr<Store> m_store;

   public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint,
             std::shared_ptr<Store> store);
    void run();

   private:
    void do_accept();

    void on_accept(beast::error_code ec, tcp::socket socket);
};
}  // namespace rest
