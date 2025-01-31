#include <algorithm>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "server.hpp"

namespace beast = boost::beast;    // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

int main(int argc, char* argv[]) {
    // Check command line arguments.
    if (argc != 5) {
        std::cerr << "Usage: http-server-async <address> <port> <doc_root> "
                     "<threads>\n"
                  << "Example:\n"
                  << "    http-server-async 0.0.0.0 8080 . 1\n";
        return EXIT_FAILURE;
    }
    std::string const address = argv[1];
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const threads = std::max<int>(1, std::atoi(argv[4]));

    rest::Server s(address, port, threads);
    s.run();

    return EXIT_SUCCESS;
}