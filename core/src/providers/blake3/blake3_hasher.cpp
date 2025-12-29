#include "fo/providers/blake3/blake3_hasher.hpp"
#include "fo/core/provider.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>

#ifdef FO_HAVE_BLAKE3

namespace fo::providers::blake3 {
    struct Blake3Hasher::Impl {
        blake3_hasher hasher;
    };

    Blake3Hasher::Blake3Hasher() : impl_(std::make_unique<Impl>()) {
        blake3_hasher_init(&impl_->hasher);
    }

    Blake3Hasher::~Blake3Hasher() = default;

    std::string Blake3Hasher::fast64(const std::filesystem::path& p) {
        // Fallback or implementation of fast hash using blake3 (or not appropriate)
        (void)p;
        return ""; 
    }

    std::optional<std::string> Blake3Hasher::strong(const std::filesystem::path& p) {
        // Implementation of strong hash
        // Using BLAKE3 to hash the file content
         std::ifstream file(p, std::ios::binary);
        if (!file) return std::nullopt;

        blake3_hasher hasher;
        blake3_hasher_init(&hasher);

        char buffer[8192];
        while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
            blake3_hasher_update(&hasher, buffer, file.gcount());
            if (file.eof()) break;
        }

        uint8_t output[BLAKE3_OUT_LEN];
        blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);

        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < BLAKE3_OUT_LEN; ++i) {
            ss << std::setw(2) << static_cast<int>(output[i]);
        }
        return ss.str();
    }

    std::string Blake3Hasher::strong_algo() const {
        return "blake3";
    }
}

namespace {
    [[maybe_unused]] auto reg =
        fo::core::Provider<fo::core::IHasher, fo::providers::blake3::Blake3Hasher>("blake3");
}

#endif
