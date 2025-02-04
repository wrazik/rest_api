#include "store.hpp"

#include <nlohmann/json.hpp>

namespace rest {
Event parse_event(const http::request<http::string_body> &req) {
    const auto body = nlohmann::json::parse(req.body());
    return Event{
        .date = body["date"].get<std::chrono::system_clock::time_point>(),
        .values = body["values"].get<std::vector<size_t>>()};
}

void Store::save_event(const std::string &name, const Event &event) {
    m_store[name].push_back(event);
}

double Store::get_mean(const std::string &name,
                       const std::chrono::system_clock::time_point &from,
                       const std::chrono::system_clock::time_point &to) {
    if (m_store.find(name) == m_store.end()) {
        return 0;
    }
    double sum = 0;
    size_t count = 0;
    for (const auto &event : m_store[name]) {
        if (event.date >= from && event.date <= to) {
            for (const auto &value : event.values) {
                sum += value;
                count++;
            }
        }
    }
    return sum / count;
}
const std::map<std::string, std::vector<Event>> &Store::get_store() const {
    return m_store;
}
}  // namespace rest