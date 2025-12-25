#include "fo/core/duplicate_repository.hpp"
#include <sqlite3.h>
#include <stdexcept>

namespace fo::core {

DuplicateRepository::DuplicateRepository(DatabaseManager& db) : db_(db) {}

int64_t DuplicateRepository::create_group(int64_t primary_file_id) {
    std::string sql = "INSERT INTO duplicate_groups (primary_file_id) VALUES (?) RETURNING id;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Prepare failed");
    }
    sqlite3_bind_int64(stmt, 1, primary_file_id);
    
    int64_t id = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        id = sqlite3_column_int64(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return id;
}

void DuplicateRepository::add_member(int64_t group_id, int64_t file_id) {
    std::string sql = "INSERT OR IGNORE INTO duplicate_members (group_id, file_id) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return;
    
    sqlite3_bind_int64(stmt, 1, group_id);
    sqlite3_bind_int64(stmt, 2, file_id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void DuplicateRepository::clear_all() {
    db_.execute("DELETE FROM duplicate_groups;"); // Cascade deletes members
}

std::vector<DuplicateGroupDB> DuplicateRepository::get_all_groups() {
    std::vector<DuplicateGroupDB> groups;
    
    // Get groups
    std::string sql = "SELECT id, primary_file_id FROM duplicate_groups;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return groups;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        DuplicateGroupDB g;
        g.id = sqlite3_column_int64(stmt, 0);
        g.primary_file_id = sqlite3_column_int64(stmt, 1);
        groups.push_back(g);
    }
    sqlite3_finalize(stmt);

    // Get members for each group
    for (auto& g : groups) {
        std::string msql = "SELECT file_id FROM duplicate_members WHERE group_id = ?;";
        sqlite3_stmt* mstmt;
        if (sqlite3_prepare_v2(db_.get_db(), msql.c_str(), -1, &mstmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int64(mstmt, 1, g.id);
            while (sqlite3_step(mstmt) == SQLITE_ROW) {
                g.member_ids.push_back(sqlite3_column_int64(mstmt, 0));
            }
            sqlite3_finalize(mstmt);
        }
    }
    return groups;
}

} // namespace fo::core
