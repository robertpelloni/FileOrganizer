#include "fo/core/file_repository.hpp"
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>

namespace fo::core {

// Helper to convert file_time to unix timestamp (seconds)
static int64_t to_unix(std::chrono::file_clock::time_point tp) {
    // C++20 clock_cast
    // If this fails to compile on some platforms, we might need ifdefs.
    // For now, assuming C++20 compliant stdlib.
    try {
        auto sys = std::chrono::clock_cast<std::chrono::system_clock>(tp);
        return std::chrono::system_clock::to_time_t(sys);
    } catch (...) {
        return 0;
    }
}

// Helper to convert unix timestamp to file_time
static std::chrono::file_clock::time_point from_unix(int64_t t) {
    auto sys = std::chrono::system_clock::from_time_t(static_cast<std::time_t>(t));
    return std::chrono::clock_cast<std::chrono::file_clock>(sys);
}

FileRepository::FileRepository(DatabaseManager& db) : db_(db) {}

void FileRepository::upsert(FileInfo& file) {
    std::string sql = "INSERT INTO files (path, size, mtime, is_dir) VALUES (?, ?, ?, ?) "
                      "ON CONFLICT(path) DO UPDATE SET size=excluded.size, mtime=excluded.mtime, is_dir=excluded.is_dir "
                      "RETURNING id;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare upsert: " + std::string(sqlite3_errmsg(db_.get_db())));
    }

    std::string path_str = file.path.string();
    sqlite3_bind_text(stmt, 1, path_str.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, static_cast<sqlite3_int64>(file.size));
    sqlite3_bind_int64(stmt, 3, static_cast<sqlite3_int64>(to_unix(file.mtime)));
    sqlite3_bind_int(stmt, 4, file.is_dir ? 1 : 0);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        file.id = sqlite3_column_int64(stmt, 0);
    } else {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute upsert: " + std::string(sqlite3_errmsg(db_.get_db())));
    }
    sqlite3_finalize(stmt);
}

std::optional<FileInfo> FileRepository::get_by_path(const std::filesystem::path& path) {
    std::string sql = "SELECT id, size, mtime, is_dir FROM files WHERE path = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return std::nullopt;

    std::string path_str = path.string();
    sqlite3_bind_text(stmt, 1, path_str.c_str(), -1, SQLITE_STATIC);

    std::optional<FileInfo> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        FileInfo fi;
        fi.path = path;
        fi.id = sqlite3_column_int64(stmt, 0);
        fi.size = static_cast<std::uintmax_t>(sqlite3_column_int64(stmt, 1));
        fi.mtime = from_unix(sqlite3_column_int64(stmt, 2));
        fi.is_dir = sqlite3_column_int(stmt, 3) != 0;
        result = fi;
    }
    sqlite3_finalize(stmt);
    return result;
}

void FileRepository::add_hash(int64_t file_id, const std::string& algo, const std::string& value) {
    std::string sql = "INSERT INTO file_hashes (file_id, algo, value) VALUES (?, ?, ?) "
                      "ON CONFLICT(file_id, algo) DO UPDATE SET value=excluded.value;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) throw std::runtime_error("Prepare failed");

    sqlite3_bind_int64(stmt, 1, file_id);
    sqlite3_bind_text(stmt, 2, algo.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, value.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string err = sqlite3_errmsg(db_.get_db());
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to add hash: " + err);
    }
    sqlite3_finalize(stmt);
}

std::vector<std::pair<std::string, std::string>> FileRepository::get_hashes(int64_t file_id) {
    std::vector<std::pair<std::string, std::string>> out;
    std::string sql = "SELECT algo, value FROM file_hashes WHERE file_id = ?;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return out;

    sqlite3_bind_int64(stmt, 1, file_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string algo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string val = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        out.emplace_back(algo, val);
    }
    sqlite3_finalize(stmt);
    return out;
}

} // namespace fo::core
