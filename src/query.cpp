#include "query.hpp"

namespace rest {
void from_json(const nlohmann::json& j, Query& q) {
    const auto from_millis = [](const long long val) {
        return std::chrono::system_clock::time_point(
            std::chrono::milliseconds(val));
    };
    const auto from_secs = [](const long long val) {
        return std::chrono::system_clock::time_point(std::chrono::seconds(val));
    };
    const auto unit = from_string(j.at("resultUnit").get<std::string>());

    if (j.contains("startTimestamp")) {
        if (unit == ResultUnit::SECONDS) {
            q.from = from_secs(j.at("startTimestamp").get<long long>());
        } else {
            q.from = from_millis(j.at("startTimestamp").get<long long>());
        }
    }
    if (j.contains("endTimestamp")) {
        if (unit == ResultUnit::SECONDS) {
            q.to = from_secs(j.at("endTimestamp").get<long long>());
        } else {
            q.to = from_millis(j.at("endTimestamp").get<long long>());
        }
    }
}
}  // namespace rest