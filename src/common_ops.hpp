#pragma once
#include <string>

#include <boost/beast/core/error.hpp>
#include <boost/beast/core/string_type.hpp>

namespace rest {
void fail(boost::beast::error_code ec, char const *what);
std::string get_event_name(boost::beast::string_view path);
}  // namespace rest
