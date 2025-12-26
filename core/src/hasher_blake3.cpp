#include "fo/providers/hasher_blake3.hpp"
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>

#ifdef FO_HAVE_BLAKE3
#include <blake3.h>
#endif

namespace fo::providers {

std::string Blake3Hasher::fast64(const std::filesystem::path& p) {
#ifdef FO_HAVE_BLAKE3
    // For fast64, we can just hash the first 64KB + size + mtime using BLAKE3
    // But to match the interface which expects a string, we return hex.
    // However, fast64 usually implies a specific sampling strategy.
    // If we want to use BLAKE3 for the *algorithm* but keep the sampling:
    // We should read the chunks and feed them to BLAKE3.
    
    // For now, let's just implement a full hash for "strong" and a partial for "fast64"
    // using the same sampling logic as Fast64Hasher but with BLAKE3.
    
    // Actually, let's just do a simple partial read.
    std::ifstream f(p, std::ios::binary);
    if (!f) return "";

    blake3_hasher hasher;
    blake3_hasher_init(&hasher);

    char buffer[8192];
    f.read(buffer, sizeof(buffer));
    blake3_hasher_update(&hasher, buffer, f.gcount());

    // If file is large, seek to middle and end
    auto size = std::filesystem::file_size(p);
    if (size > 65536) {
        f.seekg(size / 2);
        f.read(buffer, sizeof(buffer));
        blake3_hasher_update(&hasher, buffer, f.gcount());
        
        f.seekg(-8192, std::ios::end);
        f.read(buffer, sizeof(buffer));
        blake3_hasher_update(&hasher, buffer, f.gcount());
    }

    uint8_t output[BLAKE3_OUT_LEN];
    blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);

    std::stringstream ss;
    for(int i=0; i<8; ++i) // Just first 8 bytes for "fast64" equivalent? Or full hash?
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)output[i];
    return ss.str();
#else
    (void)p;
    return "";
#endif
}

std::optional<std::string> Blake3Hasher::strong(const std::filesystem::path& p) {
#ifdef FO_HAVE_BLAKE3
    std::ifstream f(p, std::ios::binary);
    if (!f) return std::nullopt;

    blake3_hasher hasher;
    blake3_hasher_init(&hasher);

    char buffer[65536];
    while (f.read(buffer, sizeof(buffer))) {
        blake3_hasher_update(&hasher, buffer, f.gcount());
    }
    if (f.gcount() > 0) {
        blake3_hasher_update(&hasher, buffer, f.gcount());
    }

    uint8_t output[BLAKE3_OUT_LEN];
    blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);

    std::stringstream ss;
    for(int i=0; i<BLAKE3_OUT_LEN; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)output[i];
    return ss.str();
#else
    (void)p;
    return std::nullopt;
#endif
}

} // namespace fo::providers
