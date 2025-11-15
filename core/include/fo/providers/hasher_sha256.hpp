#pragma once

#include "fo/core/interfaces.hpp"

namespace fo::providers {

// SHA-256 strong hasher using hash-library (already vendored)
class SHA256Hasher : public fo::core::IHasher {
public:
    std::string name() const override { return "sha256"; }
    std::string fast64(const std::filesystem::path& p) override;
    std::optional<std::string> strong(const std::filesystem::path& p) override;
    std::string strong_algo() const override { return "SHA-256"; }
};

} // namespace fo::providers
