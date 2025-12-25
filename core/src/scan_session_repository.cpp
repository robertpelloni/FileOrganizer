#include "fo/core/scan_session_repository.hpp"
#include <sqlite3.h>
#include <stdexcept>

namespace fo::core {

ScanSessionRepository::ScanSessionRepository(DatabaseManager& db) : db_(db) {}

int64_t ScanSessionRepository::start_session() {
    std::string sql = "INSERT INTO scan_sessions (start_time, status) VALUES (?, 'running') RETURNING id;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return 0;
    
    auto now = std::chrono::system_clock::now();
    int64_t ts = std::chrono::system_clock::to_time_t(now);
    
    sqlite3_bind_int64(stmt, 1, ts);
    
    int64_t id = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        id = sqlite3_column_int64(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return id;
}

void ScanSessionRepository::end_session(int64_t id, const std::string& status, int scanned_count) {
    std::string sql = "UPDATE scan_sessions SET end_time = ?, status = ?, scanned_count = ?, duration_ms = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return;
    
    auto now = std::chrono::system_clock::now();
    int64_t ts = std::chrono::system_clock::to_time_t(now);
    
    // Calculate duration (approximate, since we didn't store start time in memory, but we could query it. 
    // For simplicity, let's just update end_time and let app logic handle duration if needed, 
    // or query start_time.
    // Let's query start_time first.
    int64_t start_time = 0;
    {
        sqlite3_stmt* qstmt;
        if (sqlite3_prepare_v2(db_.get_db(), "SELECT start_time FROM scan_sessions WHERE id = ?", -1, &qstmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int64(qstmt, 1, id);
            if (sqlite3_step(qstmt) == SQLITE_ROW) {
                start_time = sqlite3_column_int64(qstmt, 0);
            }
            sqlite3_finalize(qstmt);
        }
    }
    
    int64_t duration = (ts - start_time) * 1000; // seconds to ms (rough)

    sqlite3_bind_int64(stmt, 1, ts);
    sqlite3_bind_text(stmt, 2, status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, scanned_count);
    sqlite3_bind_int64(stmt, 4, duration);
    sqlite3_bind_int64(stmt, 5, id);
    
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

} // namespace fo::core
