#include "store.hpp"

#include <algorithm>
#include <numeric>
#include <ranges>

#include <nlohmann/json.hpp>

#include "query.hpp"

namespace rest {
Event parse_event(const http::request<http::string_body>& req) {
    const auto body = nlohmann::json::parse(req.body());
    return Event{
        .date = body["date"].get<std::chrono::system_clock::time_point>(),
        .values = body["values"].get<std::vector<size_t>>()};
}

void Store::save_event(const std::string& name, const Event& event) {
    m_store[name].push_back(event);
}

double Store::get_mean(const std::string& name, const Query& query) {
    if (m_store.find(name) == m_store.end()) {
        return 0;
    }

    auto& events = m_store[name];
    using std::views::all;
    using std::views::filter;
    using std::views::join;
    using std::views::transform;
    // clang-format off
    auto filtered_events = events
        | filter([&](const auto& event) { return !query.from || event.date >= *query.from; })
        | filter([&](const auto& event) { return !query.to || event.date <= *query.to; });

    auto values = filtered_events
        | transform([](const auto& event) { return all(event.values); })
        | join;
    // clang-format on

    const auto [sum, count] = std::ranges::fold_left(
        values, std::pair{0.0, 0}, [](auto acc, double val) {
            return std::pair{acc.first + val, acc.second + 1};
        });
    return count == 0 ? 0.0 : sum / count;
}

const std::map<std::string, std::vector<Event>>& Store::get_store() const {
    return m_store;
}
}  // namespace rest