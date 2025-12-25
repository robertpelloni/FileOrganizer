#pragma once
#include "fo/core/database.hpp"
#include <vector>
#include <string>

namespace fo::core {

struct IgnoreRule {
    int64_t id = 0;
    std::string pattern;
    std::string reason;
};

class IgnoreRepository {
public:
    explicit IgnoreRepository(DatabaseManager& db);

    void add(const std::string& pattern, const std::string& reason = "");
    void remove(const std::string& pattern);
    std::vector<IgnoreRule> get_all();
    bool is_ignored(const std::string& path); // Simple check against patterns

private:
    DatabaseManager& db_;
    // Cache could be added here for performance
};

} // namespace fo::core
