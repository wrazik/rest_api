#pragma once
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/string_type.hpp>

namespace rest {
void fail(boost::beast::error_code ec, char const *what);
boost::beast::string_view mime_type(boost::beast::string_view path);
}  // namespace rest
