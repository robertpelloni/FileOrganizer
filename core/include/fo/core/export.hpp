#pragma once

#include "types.hpp"
#include "interfaces.hpp"
#include <string>
#include <vector>
#include <ostream>
#include <filesystem>

namespace fo::core {

// Statistics about a scan session
struct ScanStats {
    size_t total_files = 0;
    size_t total_directories = 0;
    std::uintmax_t total_size = 0;
    size_t duplicate_groups = 0;
    size_t duplicate_files = 0;
    std::uintmax_t duplicate_size = 0;
};

// Export format options
enum class ExportFormat {
    JSON,
    CSV,
    HTML
};

// Main exporter class
class Exporter {
public:
    // Export scan results to JSON
    static void to_json(std::ostream& out, 
                        const std::vector<FileInfo>& files,
                        const std::vector<DuplicateGroup>& duplicates,
                        const ScanStats& stats);
    
    // Export scan results to CSV
    static void to_csv(std::ostream& out, 
                       const std::vector<FileInfo>& files);
    
    // Export duplicate groups to CSV
    static void duplicates_to_csv(std::ostream& out, 
                                  const std::vector<DuplicateGroup>& duplicates);
    
    // Export scan results to HTML
    static void to_html(std::ostream& out,
                        const std::vector<FileInfo>& files,
                        const std::vector<DuplicateGroup>& duplicates,
                        const ScanStats& stats,
                        bool include_thumbnails = false);
    
    // Convenience: export to file
    static bool export_to_file(const std::filesystem::path& output_path,
                               const std::vector<FileInfo>& files,
                               const std::vector<DuplicateGroup>& duplicates,
                               const ScanStats& stats,
                               ExportFormat format,
                               bool include_thumbnails = false);
    
    // Compute statistics from scan results
    static ScanStats compute_stats(const std::vector<FileInfo>& files,
                                   const std::vector<DuplicateGroup>& duplicates);
    
    // Format file size for human readability
    static std::string format_size(std::uintmax_t bytes);

    // Format timestamp to ISO 8601
    static std::string format_time(std::chrono::file_clock::time_point tp);

    // Escape string for JSON output
    static std::string json_escape(const std::string& s);

    // Escape string for CSV output
    static std::string csv_escape(const std::string& s);

    // Escape string for HTML output
    static std::string html_escape(const std::string& s);
};

} // namespace fo::core

