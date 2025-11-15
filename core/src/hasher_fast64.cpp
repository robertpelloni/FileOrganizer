#include "fo/core/interfaces.hpp"
#include "fo/core/registry.hpp"
#include <fstream>
#include <array>
#include <iomanip>
#include <sstream>

namespace fo::core {

static std::string to_hex64(uint64_t v) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(16) << v;
    return oss.str();
}

class Fast64Hasher : public IHasher {
public:
    std::string name() const override { return "fast64"; }

    // Non-cryptographic: sample up to first/middle/last 16KB and mix
    std::string fast64(const std::filesystem::path& p) override {
        std::ifstream f(p, std::ios::binary);
        if (!f) return {};
        f.seekg(0, std::ios::end);
        std::streamoff len = f.tellg();
        f.seekg(0, std::ios::beg);
        const size_t chunk = 16 * 1024;

        auto mix = [](uint64_t h, const unsigned char* data, size_t n) {
            // simple 64-bit mixer (xorshift + multiply)
            const uint64_t m = 0x9E3779B97F4A7C15ull;
            for (size_t i = 0; i < n; ++i) {
                h ^= data[i];
                h *= m;
                h ^= (h >> 33);
            }
            return h;
        };

        std::array<unsigned char, 16 * 1024> buf{};
        uint64_t h = 1469598103934665603ull; // FNV offset basis as seed

        auto read_at = [&](std::streamoff pos, size_t n) {
            if (pos < 0 || pos >= len) return;
            f.seekg(pos, std::ios::beg);
            f.read(reinterpret_cast<char*>(buf.data()), std::min(n, static_cast<size_t>(len - pos)));
            auto got = static_cast<size_t>(f.gcount());
            h = mix(h, buf.data(), got);
        };

        if (len <= static_cast<std::streamoff>(chunk * 3)) {
            // small file: read whole
            f.seekg(0, std::ios::beg);
            while (f) {
                f.read(reinterpret_cast<char*>(buf.data()), buf.size());
                auto got = static_cast<size_t>(f.gcount());
                if (!got) break;
                h = mix(h, buf.data(), got);
            }
        } else {
            read_at(0, chunk);
            read_at(len / 2 - static_cast<std::streamoff>(chunk) / 2, chunk);
            read_at(len - static_cast<std::streamoff>(chunk), chunk);
        }
        return to_hex64(h);
    }
};

// Static registration
static bool reg_fast64 = [](){
    Registry<IHasher>::instance().add("fast64", [](){ return std::make_unique<Fast64Hasher>(); });
    return true;
}();

} // namespace fo::core
