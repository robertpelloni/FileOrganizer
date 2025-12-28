#include "fo/core/ads_cache.hpp"
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fo::core {

bool ADSCache::is_supported([[maybe_unused]] const std::filesystem::path& path) {
#ifdef _WIN32
    // Check if the path is on an NTFS volume
    std::wstring root = path.root_path().wstring();
    if (root.empty()) {
        // Relative path - get current directory's root
        wchar_t cwd[MAX_PATH];
        if (GetCurrentDirectoryW(MAX_PATH, cwd)) {
            root = std::filesystem::path(cwd).root_path().wstring();
        }
    }
    
    wchar_t fs_name[MAX_PATH];
    if (GetVolumeInformationW(root.c_str(), nullptr, 0, nullptr, nullptr, nullptr, fs_name, MAX_PATH)) {
        return wcscmp(fs_name, L"NTFS") == 0 || wcscmp(fs_name, L"ReFS") == 0;
    }
    return false;
#else
    return false;
#endif
}

std::optional<std::string> ADSCache::get_hash(
    const std::filesystem::path& path,
    const std::string& hash_type) {
#ifdef _WIN32
    if (!is_supported(path)) return std::nullopt;
    
    // Build ADS path: file.jpg:fo_cache
    std::wstring ads_path = path.wstring() + L":fo_cache";
    
    HANDLE hFile = CreateFileW(
        ads_path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);
    
    if (hFile == INVALID_HANDLE_VALUE) return std::nullopt;
    
    // Read the cache content
    char buffer[4096];
    DWORD bytesRead = 0;
    if (!ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) {
        CloseHandle(hFile);
        return std::nullopt;
    }
    CloseHandle(hFile);
    buffer[bytesRead] = '\0';
    
    // Parse format: mtime_ns|hash_type|hash_value\n...
    std::string content(buffer);
    std::istringstream iss(content);
    std::string line;
    
    // Get current file mtime
    std::error_code ec;
    auto current_mtime = std::filesystem::last_write_time(path, ec);
    if (ec) return std::nullopt;
    
    auto current_mtime_ns = current_mtime.time_since_epoch().count();
    
    while (std::getline(iss, line)) {
        // Parse: mtime_ns|hash_type|hash_value
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        if (pos1 == std::string::npos || pos2 == std::string::npos) continue;
        
        int64_t stored_mtime = std::stoll(line.substr(0, pos1));
        std::string stored_type = line.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string stored_hash = line.substr(pos2 + 1);
        
        // Check if mtime matches and hash type matches
        if (stored_mtime == current_mtime_ns && stored_type == hash_type) {
            return stored_hash;
        }
    }
    
    return std::nullopt;
#else
    (void)path; (void)hash_type;
    return std::nullopt;
#endif
}

bool ADSCache::set_hash(
    const std::filesystem::path& path,
    const std::string& hash_type,
    const std::string& hash_value) {
#ifdef _WIN32
    if (!is_supported(path)) return false;
    
    // Get current file mtime
    std::error_code ec;
    auto current_mtime = std::filesystem::last_write_time(path, ec);
    if (ec) return false;
    
    auto mtime_ns = current_mtime.time_since_epoch().count();
    
    // Build cache line
    std::ostringstream oss;
    oss << mtime_ns << "|" << hash_type << "|" << hash_value << "\n";
    std::string cache_line = oss.str();
    
    // Build ADS path
    std::wstring ads_path = path.wstring() + L":fo_cache";
    
    // Read existing content (to preserve other hash types)
    std::string existing_content;
    {
        HANDLE hRead = CreateFileW(ads_path.c_str(), GENERIC_READ, FILE_SHARE_READ,
            nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hRead != INVALID_HANDLE_VALUE) {
            char buffer[4096];
            DWORD bytesRead = 0;
            if (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) {
                buffer[bytesRead] = '\0';
                existing_content = buffer;
            }
            CloseHandle(hRead);
        }
    }
    
    // Filter out old entries for this hash type
    std::ostringstream new_content;
    std::istringstream iss(existing_content);
    std::string line;
    while (std::getline(iss, line)) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            std::string stored_type = line.substr(pos1 + 1, pos2 - pos1 - 1);
            if (stored_type != hash_type) {
                new_content << line << "\n";
            }
        }
    }
    new_content << cache_line;
    
    // Write to ADS
    HANDLE hFile = CreateFileW(ads_path.c_str(), GENERIC_WRITE, 0,
        nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) return false;
    
    std::string final_content = new_content.str();
    DWORD bytesWritten = 0;
    bool success = WriteFile(hFile, final_content.c_str(), 
        static_cast<DWORD>(final_content.size()), &bytesWritten, nullptr);
    CloseHandle(hFile);
    
    return success && bytesWritten == final_content.size();
#else
    (void)path; (void)hash_type; (void)hash_value;
    return false;
#endif
}

bool ADSCache::clear(const std::filesystem::path& path) {
#ifdef _WIN32
    if (!is_supported(path)) return false;

    std::wstring ads_path = path.wstring() + L":fo_cache";
    return DeleteFileW(ads_path.c_str()) != 0;
#else
    (void)path;
    return false;
#endif
}

std::optional<std::chrono::file_clock::time_point> ADSCache::get_cached_mtime(
    const std::filesystem::path& path) {
#ifdef _WIN32
    if (!is_supported(path)) return std::nullopt;

    std::wstring ads_path = path.wstring() + L":fo_cache";

    HANDLE hFile = CreateFileW(
        ads_path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE) return std::nullopt;

    char buffer[256];
    DWORD bytesRead = 0;
    if (!ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) {
        CloseHandle(hFile);
        return std::nullopt;
    }
    CloseHandle(hFile);
    buffer[bytesRead] = '\0';

    // Parse first line to get mtime
    std::string content(buffer);
    size_t pos = content.find('|');
    if (pos == std::string::npos) return std::nullopt;

    try {
        int64_t mtime_ns = std::stoll(content.substr(0, pos));
        return std::chrono::file_clock::time_point(
            std::chrono::file_clock::duration(mtime_ns));
    } catch (...) {
        return std::nullopt;
    }
#else
    (void)path;
    return std::nullopt;
#endif
}

} // namespace fo::core

