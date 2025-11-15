#pragma once

#include "fo/core/interfaces.hpp"

namespace fo::providers {

// xxHash64 hasher (requires xxHash library from libs/xxHash or vcpkg)
// Fast non-cryptographic hash; good for prefiltering and duplicate detection
class XXHasher : public fo::core::IHasher {
public:
    std::string name() const override { return "xxhash64"; }
    std::string fast64(const std::filesystem::path& p) override;
    std::optional<std::string> strong(const std::filesystem::path& p) override { return fast64(p); }
    std::string strong_algo() const override { return "XXH64"; }
};

} // namespace fo::providers
