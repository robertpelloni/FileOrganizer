#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <filesystem>

namespace fo::core {

struct FileInfo {
    std::filesystem::path path;
    std::uintmax_t size = 0;
    std::chrono::file_clock::time_point mtime{};
};

struct DateMetadata {
    // Normalized primary datetime if available (UTC)
    std::chrono::system_clock::time_point taken{};
    bool has_taken = false;
    // Original string if parsed from filename or tag (for logging/DB)
    std::string source_string;
    std::string source_field; // e.g., EXIF:DateTimeOriginal, Filename, etc.
};

struct ImageMetadata {
    DateMetadata date;
    bool has_gps = false;
    double gps_lat = 0.0;
    double gps_lon = 0.0;
};

struct Hashes {
    // Non-cryptographic quick hash suitable for prefiltering
    std::string fast64;
    // Optional strong hash, empty if not computed
    std::string strong;
    std::string strong_algo;
};

} // namespace fo::core
