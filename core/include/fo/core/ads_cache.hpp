#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <chrono>

namespace fo::core {

/**
 * @brief Alternate Data Streams (ADS) cache for Windows NTFS.
 * 
 * On NTFS, files can have multiple "streams" of data. The main content is the
 * unnamed stream, but we can create named streams like "file.jpg:fo_hash" to
 * store metadata without modifying the file's main content.
 * 
 * This class provides a caching mechanism for file hashes using ADS.
 * On non-Windows platforms or non-NTFS filesystems, operations are no-ops.
 */
class ADSCache {
public:
    static constexpr const char* STREAM_NAME = ":fo_cache";
    
    /**
     * @brief Check if ADS is supported for the given path.
     * @param path File path to check.
     * @return true if ADS is supported (Windows + NTFS), false otherwise.
     */
    static bool is_supported(const std::filesystem::path& path);
    
    /**
     * @brief Get cached hash for a file.
     * @param path File path.
     * @param hash_type Type of hash (e.g., "fast64", "sha256", "blake3").
     * @return Cached hash value if valid, nullopt if not cached or stale.
     */
    static std::optional<std::string> get_hash(
        const std::filesystem::path& path,
        const std::string& hash_type);
    
    /**
     * @brief Store hash in ADS cache.
     * @param path File path.
     * @param hash_type Type of hash.
     * @param hash_value Hash value to store.
     * @return true if stored successfully, false otherwise.
     */
    static bool set_hash(
        const std::filesystem::path& path,
        const std::string& hash_type,
        const std::string& hash_value);
    
    /**
     * @brief Clear all cached data for a file.
     * @param path File path.
     * @return true if cleared successfully, false otherwise.
     */
    static bool clear(const std::filesystem::path& path);
    
    /**
     * @brief Get the mtime stored with the cached hash.
     * @param path File path.
     * @return Stored mtime if available, nullopt otherwise.
     */
    static std::optional<std::chrono::file_clock::time_point> get_cached_mtime(
        const std::filesystem::path& path);
};

} // namespace fo::core

