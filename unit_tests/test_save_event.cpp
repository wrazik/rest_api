#include <gtest/gtest.h>
#include <store.hpp>

namespace rest {

TEST(StoreTest, ParseEvent) {
    std::string event = R"({"timestamp": 1234567890, "x": 1, "y": 2, "z": 3})";
    auto parsed_event = rest::parse_event(event);
    ASSERT_EQ(parsed_event.timestamp, 1234567890);
    ASSERT_EQ(parsed_event.x, 1);
    ASSERT_EQ(parsed_event.y, 2);
    ASSERT_EQ(parsed_event.z, 3);
}

}  // namespace rest
