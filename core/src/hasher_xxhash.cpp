#include "fo/providers/hasher_xxhash.hpp"

// Using XXH64 from vendored xxHash
#define XXH_INLINE_ALL
#include "../../libs/xxHash/xxhash.h"

#include <fstream>
#include <array>
#include <iomanip>
#include <sstream>

namespace fo::providers {

std::string XXHasher::fast64(const std::filesystem::path& p) {
    std::ifstream f(p, std::ios::binary);
    if (!f) return {};

    XXH64_state_t state;
    XXH64_reset(&state, 0);

    std::array<char, 64 * 1024> buf;
    while (f) {
        f.read(buf.data(), buf.size());
        auto got = static_cast<size_t>(f.gcount());
        if (got == 0) break;
        XXH64_update(&state, buf.data(), got);
    }

    XXH64_hash_t h = XXH64_digest(&state);
    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(16) << h;
    return oss.str();
}

} // namespace fo::providers
