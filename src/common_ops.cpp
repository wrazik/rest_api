#include "common_ops.hpp"

#include <iostream>

#include <boost/beast/core/string.hpp>

namespace rest {
void fail(boost::beast::error_code ec, char const *what) {
    std::cerr << what << ": " << ec.message() << "\n";
}

std::string get_event_name(boost::beast::string_view path) {
    size_t start = path.find('/', 1) + 1;
    size_t end = path.find('/', start);

    return path.substr(start, end - start);
}

}  // namespace rest
