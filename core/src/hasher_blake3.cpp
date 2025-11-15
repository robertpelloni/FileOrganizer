#include "fo/providers/hasher_blake3.hpp"

// TODO: Implement BLAKE3 provider when building with BLAKE3 support
// Requires: vcpkg install blake3
// Then: #include <blake3.h>
// Feature-guard with #ifdef FO_HAVE_BLAKE3

namespace fo::providers {

std::string Blake3Hasher::fast64(const std::filesystem::path& p) {
    (void)p;
    // Placeholder: not implemented without BLAKE3
    return {};
}

std::optional<std::string> Blake3Hasher::strong(const std::filesystem::path& p) {
    (void)p;
    // Placeholder: not implemented without BLAKE3
    return std::nullopt;
}

} // namespace fo::providers
