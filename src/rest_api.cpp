#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "server.hpp"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: rest_api <address> <port> "
                     "<threads>\n"
                  << "Example:\n"
                  << "    http-server-async 0.0.0.0 8080 . 1\n";
        return EXIT_FAILURE;
    }
    std::string const address = argv[1];
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const threads = std::max<int>(1, std::atoi(argv[3]));

    rest::Server s(address, port, threads);
    s.run();

    return EXIT_SUCCESS;
}