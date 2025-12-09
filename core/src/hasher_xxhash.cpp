#include "fo/core/interfaces.hpp"
#include "fo/core/registry.hpp"

// Using XXH64 from vendored xxHash
#define XXH_INLINE_ALL
#include "../../libs/xxHash/xxhash.h"

#include <fstream>
#include <array>
#include <iomanip>
#include <sstream>

namespace fo::core {

class XXHasher : public IHasher {
public:
    std::string name() const override { return "xxhash"; }
    std::string fast64(const std::filesystem::path& p) override;
};

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

// Static registration
static bool reg_hasher_xxhash = [](){
    Registry<IHasher>::instance().add("xxhash", [](){ return std::make_unique<XXHasher>(); });
    return true;
}();

void register_hasher_xxhash() { (void)reg_hasher_xxhash; }

} // namespace fo::core
