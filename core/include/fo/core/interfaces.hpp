#pragma once

#include "types.hpp"
#include <string>
#include <vector>
#include <optional>

namespace fo::core {

class IFileScanner {
public:
    virtual ~IFileScanner() = default;
    virtual std::string name() const = 0;
    virtual std::vector<FileInfo> scan(
        const std::vector<std::filesystem::path>& roots,
        const std::vector<std::string>& include_exts,
        bool follow_symlinks) = 0;
};

class IHasher {
public:
    virtual ~IHasher() = default;
    virtual std::string name() const = 0;
    virtual std::string fast64(const std::filesystem::path& p) = 0;
    virtual std::optional<std::string> strong(const std::filesystem::path& p) { (void)p; return std::nullopt; }
    virtual std::string strong_algo() const { return ""; }
};

class IMetadataProvider {
public:
    virtual ~IMetadataProvider() = default;
    virtual std::string name() const = 0;
    virtual bool read(const std::filesystem::path& p, ImageMetadata& out) = 0;
    virtual bool write(const std::filesystem::path& p, const ImageMetadata& in) { (void)p; (void)in; return false; }
};

struct DuplicateGroup {
    std::uintmax_t size = 0;
    std::string fast64;
    std::vector<FileInfo> files;
};

class IDuplicateFinder {
public:
    virtual ~IDuplicateFinder() = default;
    virtual std::string name() const = 0;
    virtual std::vector<DuplicateGroup> group(const std::vector<FileInfo>& files, IHasher& hasher) = 0;
};

} // namespace fo::core
