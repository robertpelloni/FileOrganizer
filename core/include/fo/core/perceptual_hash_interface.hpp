#pragma once

#include <string>
#include <filesystem>
#include <optional>
#include <vector>
#include <cstdint>
#include <bit>

namespace fo::core {

struct PerceptualHash {
    uint64_t value = 0;
    std::string method; // "dhash", "phash", "ahash"
};

class IPerceptualHasher {
public:
    virtual ~IPerceptualHasher() = default;
    virtual std::string name() const = 0;
    
    // Compute a perceptual hash for an image
    virtual std::optional<PerceptualHash> compute(const std::filesystem::path& image_path) = 0;
    
    // Calculate Hamming distance between two hashes
    static int distance(uint64_t a, uint64_t b) {
        // __builtin_popcountll is GCC/Clang, std::popcount is C++20
        // Since we are C++20:
        return std::popcount(a ^ b);
    }
};

} // namespace fo::core
