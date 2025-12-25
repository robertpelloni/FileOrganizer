#include "fo/core/ignore_repository.hpp"
#include <sqlite3.h>
#include <stdexcept>
#include <regex>

namespace fo::core {

IgnoreRepository::IgnoreRepository(DatabaseManager& db) : db_(db) {}

void IgnoreRepository::add(const std::string& pattern, const std::string& reason) {
    std::string sql = "INSERT OR IGNORE INTO ignore_list (pattern, reason) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return;
    
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, reason.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void IgnoreRepository::remove(const std::string& pattern) {
    std::string sql = "DELETE FROM ignore_list WHERE pattern = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return;
    
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::vector<IgnoreRule> IgnoreRepository::get_all() {
    std::vector<IgnoreRule> rules;
    std::string sql = "SELECT id, pattern, reason FROM ignore_list;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return rules;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        IgnoreRule r;
        r.id = sqlite3_column_int64(stmt, 0);
        r.pattern = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* reason = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        if (reason) r.reason = reason;
        rules.push_back(r);
    }
    sqlite3_finalize(stmt);
    return rules;
}

bool IgnoreRepository::is_ignored(const std::string& path) {
    // This is a naive implementation. Ideally, we should compile regexes or use glob matching.
    // For now, let's assume patterns are simple substrings or we fetch all and check.
    // Fetching all every time is slow.
    // Let's just fetch all once and cache? Or let the caller handle caching.
    // For now, let's just fetch all.
    auto rules = get_all();
    for (const auto& r : rules) {
        // Simple substring check for now, or basic glob if we had a glob library.
        // Let's assume regex for flexibility but catch errors.
        try {
            std::regex re(r.pattern);
            if (std::regex_search(path, re)) return true;
        } catch (...) {
            // Invalid regex, treat as substring?
            if (path.find(r.pattern) != std::string::npos) return true;
        }
    }
    return false;
}

} // namespace fo::core
