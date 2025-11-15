#include "fo/providers/hasher_sha256.hpp"
#include "../../libs/hash-library/sha256.h"
#include <fstream>
#include <array>

namespace fo::providers {

std::string SHA256Hasher::fast64(const std::filesystem::path& p) {
    // Reuse the fast sampling logic from hasher_fast64.cpp for prefilter
    // For simplicity, just delegate to strong() for prototyping; in production, use a separate fast sampler
    auto s = strong(p);
    return s.has_value() ? s.value().substr(0, 16) : "";
}

std::optional<std::string> SHA256Hasher::strong(const std::filesystem::path& p) {
    std::ifstream f(p, std::ios::binary);
    if (!f) return std::nullopt;

    SHA256 sha;
    std::array<char, 16 * 1024> buf;
    while (f) {
        f.read(buf.data(), buf.size());
        std::streamsize got = f.gcount();
        if (got <= 0) break;
        sha.add(buf.data(), static_cast<size_t>(got));
    }
    return sha.getHash();
}

} // namespace fo::providers
