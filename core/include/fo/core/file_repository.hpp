#pragma once
#include "fo/core/database.hpp"
#include "fo/core/types.hpp"
#include <optional>
#include <vector>

namespace fo::core {

class FileRepository {
public:
    explicit FileRepository(DatabaseManager& db);

    // Insert or update a file. Returns the file ID.
    // Updates size/mtime if path exists.
    // Sets file.id to the new/existing ID.
    void upsert(FileInfo& file);

    // Get file by path.
    std::optional<FileInfo> get_by_path(const std::filesystem::path& path);

    // Add a hash for a file.
    void add_hash(int64_t file_id, const std::string& algo, const std::string& value);

    // Get all hashes for a file.
    // Returns vector of pair<algo, value>
    std::vector<std::pair<std::string, std::string>> get_hashes(int64_t file_id);

private:
    DatabaseManager& db_;
};

} // namespace fo::core
