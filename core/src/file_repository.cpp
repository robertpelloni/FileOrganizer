#include "fo/core/file_repository.hpp"
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>
#include <bit>
#include <charconv>

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

std::optional<FileInfo> FileRepository::get_by_id(int64_t id) {
    std::string sql = "SELECT path, size, mtime, is_dir FROM files WHERE id = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return std::nullopt;

    sqlite3_bind_int64(stmt, 1, id);

    std::optional<FileInfo> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        FileInfo fi;
        fi.id = id;
        const char* path_c = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (path_c) fi.path = std::filesystem::u8path(path_c);
        fi.size = static_cast<std::uintmax_t>(sqlite3_column_int64(stmt, 1));
        fi.mtime = from_unix(sqlite3_column_int64(stmt, 2));
        fi.is_dir = sqlite3_column_int(stmt, 3) != 0;
        result = fi;
    }
    sqlite3_finalize(stmt);
    return result;
}

std::vector<int64_t> FileRepository::find_similar_images(uint64_t target_hash, int threshold) {
    std::vector<int64_t> matches;
    // Select all dhash values
    std::string sql = "SELECT file_id, value FROM file_hashes WHERE algo = 'dhash';";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return matches;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int64_t file_id = sqlite3_column_int64(stmt, 0);
        const char* val_c = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        if (!val_c) continue;

        uint64_t hash = 0;
        // Parse string to uint64
        // Assuming decimal string from std::to_string
        std::from_chars(val_c, val_c + strlen(val_c), hash);

        int dist = std::popcount(target_hash ^ hash);
        if (dist <= threshold) {
            matches.push_back(file_id);
        }
    }
    sqlite3_finalize(stmt);
    return matches;
}

void FileRepository::add_tag(int64_t file_id, const std::string& tag, double confidence, const std::string& source) {
    // 1. Ensure tag exists
    std::string sql_tag = "INSERT INTO tags (name) VALUES (?) ON CONFLICT(name) DO UPDATE SET name=name RETURNING id;";
    sqlite3_stmt* stmt_tag;
    if (sqlite3_prepare_v2(db_.get_db(), sql_tag.c_str(), -1, &stmt_tag, nullptr) != SQLITE_OK) return;
    
    sqlite3_bind_text(stmt_tag, 1, tag.c_str(), -1, SQLITE_STATIC);
    
    int64_t tag_id = 0;
    if (sqlite3_step(stmt_tag) == SQLITE_ROW) {
        tag_id = sqlite3_column_int64(stmt_tag, 0);
    }
    sqlite3_finalize(stmt_tag);
    
    if (tag_id == 0) {
        // Fallback: select id if insert failed (shouldn't happen with RETURNING but just in case)
        std::string sql_sel = "SELECT id FROM tags WHERE name = ?;";
        sqlite3_stmt* stmt_sel;
        sqlite3_prepare_v2(db_.get_db(), sql_sel.c_str(), -1, &stmt_sel, nullptr);
        sqlite3_bind_text(stmt_sel, 1, tag.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt_sel) == SQLITE_ROW) tag_id = sqlite3_column_int64(stmt_sel, 0);
        sqlite3_finalize(stmt_sel);
    }
    
    if (tag_id == 0) return; // Failed to get tag ID

    // 2. Link file and tag
    std::string sql_link = "INSERT INTO file_tags (file_id, tag_id, confidence, source) VALUES (?, ?, ?, ?) "
                           "ON CONFLICT(file_id, tag_id) DO UPDATE SET confidence=excluded.confidence, source=excluded.source;";
    
    sqlite3_stmt* stmt_link;
    if (sqlite3_prepare_v2(db_.get_db(), sql_link.c_str(), -1, &stmt_link, nullptr) != SQLITE_OK) return;

    sqlite3_bind_int64(stmt_link, 1, file_id);
    sqlite3_bind_int64(stmt_link, 2, tag_id);
    sqlite3_bind_double(stmt_link, 3, confidence);
    sqlite3_bind_text(stmt_link, 4, source.c_str(), -1, SQLITE_STATIC);

    sqlite3_step(stmt_link);
    sqlite3_finalize(stmt_link);
}

std::vector<std::pair<std::string, double>> FileRepository::get_tags(int64_t file_id) {
    std::vector<std::pair<std::string, double>> out;
    std::string sql = "SELECT t.name, ft.confidence FROM file_tags ft "
                      "JOIN tags t ON ft.tag_id = t.id "
                      "WHERE ft.file_id = ? ORDER BY ft.confidence DESC;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_.get_db(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return out;

    sqlite3_bind_int64(stmt, 1, file_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        double conf = sqlite3_column_double(stmt, 1);
        out.emplace_back(name, conf);
    }
    sqlite3_finalize(stmt);
    return out;
}

} // namespace fo::core
