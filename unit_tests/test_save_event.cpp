#include <gtest/gtest.h>

#include <chrono>
#include <string>
#include <vector>

#include <boost/beast/http.hpp>

#include <store.hpp>

namespace rest {
    namespace http = boost::beast::http;

TEST(StoreTest, ParseCorrectEvent) {
    http::request<http::string_body> req;
    std::string event = R"({"date": 1738696320, "values": [1, 2, 3, 4, 5]})";
    req.body() = std::move(event);
    const auto parsed_event = rest::parse_event(req);

    ASSERT_EQ(parsed_event.date, std::chrono::system_clock::time_point(std::chrono::seconds(1738696320)));
    ASSERT_EQ(parsed_event.values, std::vector<size_t>({1, 2, 3, 4, 5}));
}

TEST(StoreTest, IncorrectEventShouldThrow) {
    http::request<http::string_body> req;
    std::string event = R"({"date": 1738696320, "values": [1, 2,, 3, 4, 5]})";
    req.body() = std::move(event);
    EXPECT_THROW(rest::parse_event(req), nlohmann::json::parse_error);
}

}  // namespace rest
