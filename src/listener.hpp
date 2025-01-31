#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace rest {

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Listener : public std::enable_shared_from_this<Listener> {
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::shared_ptr<std::string const> doc_root_;

   public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint,
             std::shared_ptr<std::string const> const& doc_root);
    void run();

   private:
    void do_accept();

    void on_accept(beast::error_code ec, tcp::socket socket);
};
}  // namespace rest
