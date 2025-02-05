#pragma once
#include <chrono>
#include <optional>

#include <nlohmann/json.hpp>

namespace rest {
enum class ResultUnit { SECONDS, MILLIS };

inline ResultUnit from_string(const std::string& str) {
    if (str == "seconds") {
        return ResultUnit::SECONDS;
    } else if (str == "milliseconds") {
        return ResultUnit::MILLIS;
    }
    throw std::invalid_argument("Unknown result unit");
}

struct Query {
    std::optional<std::chrono::system_clock::time_point> from;
    std::optional<std::chrono::system_clock::time_point> to;
};

void from_json(const nlohmann::json& j, Query& q);
}  // namespace rest