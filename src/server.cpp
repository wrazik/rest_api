#include "server.hpp"

#include <algorithm>
#include <map>
#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>

#include "store.hpp"

namespace rest {
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

Server::Server(const std::string &address, const unsigned short port,
               const size_t threads)
    : m_address(address), m_context(static_cast<int>(threads)), m_thread_count(threads) {
    m_threads.reserve(threads - 1);
    net::ip::address addr = net::ip::make_address(address);

    m_listener = std::make_shared<Listener>(
        m_context, tcp::endpoint{addr, port}, std::make_shared<Store>());
}

void Server::run() {
    m_listener->run();
    for (auto i = m_thread_count - 1; i > 0; --i) {
        m_threads.emplace_back(
            [&m_context = this->m_context] { m_context.run(); });
    }
    m_context.run();
}

}  // namespace rest