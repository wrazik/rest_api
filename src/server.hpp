#pragma once
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>

#include "listener.hpp"

namespace rest {

class Server {
   public:
    Server(const std::string &address, const unsigned short port,
           const size_t threads);
    ~Server() = default;
    void run();

   private:
    std::string m_address;
    std::shared_ptr<Listener> m_listener;
    std::vector<std::thread> m_threads;
    size_t m_thread_count;
    boost::asio::io_context m_context;
    size_t m_port;
};
}  // namespace rest