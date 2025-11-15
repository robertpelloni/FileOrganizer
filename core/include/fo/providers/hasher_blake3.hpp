#pragma once

#include "fo/core/interfaces.hpp"

namespace fo::providers {

// BLAKE3 hasher (requires BLAKE3 library via vcpkg or custom build)
// Fast cryptographic hash suitable for deduplication and checksums
// vcpkg: `vcpkg install blake3`
class Blake3Hasher : public fo::core::IHasher {
public:
    std::string name() const override { return "blake3"; }
    std::string fast64(const std::filesystem::path& p) override;
    std::optional<std::string> strong(const std::filesystem::path& p) override;
    std::string strong_algo() const override { return "BLAKE3"; }
};

} // namespace fo::providers
