#pragma once

#include "interfaces.hpp"
#include "registry.hpp"
#include <memory>

namespace fo::core {

struct EngineConfig {
    std::string scanner = "std";
    std::string hasher = "fast64";
};

class Engine {
public:
    explicit Engine(EngineConfig cfg = {})
        : cfg_(std::move(cfg))
        , scanner_(Registry<IFileScanner>::instance().create(cfg_.scanner))
        , hasher_(Registry<IHasher>::instance().create(cfg_.hasher)) {}

    std::vector<FileInfo> scan(const std::vector<std::filesystem::path>& roots,
                               const std::vector<std::string>& include_exts,
                               bool follow_symlinks) {
        if (!scanner_) throw std::runtime_error("scanner not found: " + cfg_.scanner);
        return scanner_->scan(roots, include_exts, follow_symlinks);
    }

    std::vector<DuplicateGroup> find_duplicates(const std::vector<FileInfo>& files) {
        if (!hasher_) throw std::runtime_error("hasher not found: " + cfg_.hasher);
        // use size+fast64 strategy for now
        class SizeHashDuplicateFinder local;
        return local.group(files, *hasher_);
    }

    IHasher& hasher() { return *hasher_; }

private:
    // local implementation of duplicate finder from dupe_size_fast.cpp
    class SizeHashDuplicateFinder : public IDuplicateFinder {
    public:
        std::string name() const override { return "size+fast64"; }
        std::vector<DuplicateGroup> group(const std::vector<FileInfo>& files, IHasher& hasher) override;
    };

    EngineConfig cfg_{};
    std::unique_ptr<IFileScanner> scanner_{};
    std::unique_ptr<IHasher> hasher_{};
};

} // namespace fo::core
