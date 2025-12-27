#pragma once
#include "fo/core/database.hpp"
#include "fo/core/types.hpp"
#include <optional>
#include <vector>

namespace fo::core {

struct UpsertResult {
    bool is_new = false;
    bool is_modified = false;
};

class FileRepository {
public:
    explicit FileRepository(DatabaseManager& db);

    // Insert or update a file. Returns status.
    // Updates size/mtime if path exists and changed.
    // Sets file.id to the new/existing ID.
    UpsertResult upsert(FileInfo& file);

    // Prune files that are not in the given list of IDs but are within the given roots.
    void prune_missing(const std::vector<int64_t>& present_ids, const std::vector<std::filesystem::path>& roots);

    // Get file by path.
    std::optional<FileInfo> get_by_path(const std::filesystem::path& path);

    // Add a hash for a file.
    void add_hash(int64_t file_id, const std::string& algo, const std::string& value);

    // Get all hashes for a file.
    // Returns vector of pair<algo, value>
    std::vector<std::pair<std::string, std::string>> get_hashes(int64_t file_id);

    // Get file by ID.
    std::optional<FileInfo> get_by_id(int64_t id);

    // Find files with similar perceptual hash.
    // Assumes 'dhash' algorithm stored as decimal or hex string.
    // Returns list of file IDs.
    std::vector<int64_t> find_similar_images(uint64_t target_hash, int threshold);

    // Add a tag to a file.
    // source: 'user', 'ai', 'exif'
    void add_tag(int64_t file_id, const std::string& tag, double confidence = 1.0, const std::string& source = "user");

    // Get all tags for a file.
    // Returns vector of pair<tag, confidence>
    std::vector<std::pair<std::string, double>> get_tags(int64_t file_id);

private:
    DatabaseManager& db_;
};

} // namespace fo::core
