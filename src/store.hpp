#pragma once
#include <chrono>
#include <map>
#include <string>
#include <vector>

#include <boost/beast/core/string_type.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

inline bool is_milliseconds(long long timestamp) {
    return timestamp > 1'577'836'800LL * 1000;
}
namespace nlohmann {
template <>
struct adl_serializer<std::chrono::system_clock::time_point> {
    static void from_json(const json& j,
                          std::chrono::system_clock::time_point& value) {
        const auto val = j.template get<long long>();
        if (is_milliseconds(val)) {
            value = std::chrono::system_clock::time_point(
                std::chrono::milliseconds(val));
        } else {
            value = std::chrono::system_clock::time_point(
                std::chrono::seconds(val));
        }
    }
};
}  // namespace nlohmann

namespace rest {
namespace beast = boost::beast;
namespace http = beast::http;
struct Query;
struct Event {
    std::chrono::system_clock::time_point date;
    std::vector<double> values;
};

Event parse_event(const http::request<http::string_body>& req);

class Store {
   private:
    std::map<std::string, std::vector<Event>> m_store;

   public:
    Store() = default;
    ~Store() = default;
    Store(const Store&) = delete;
    Store& operator=(const Store&) = delete;
    Store(Store&&) = delete;
    Store& operator=(Store&&) = delete;

    const std::map<std::string, std::vector<Event>>& get_store() const;

    void save_event(const std::string& name, const Event& event);
    double get_mean(const std::string& name, const Query& query);
};
}  // namespace rest