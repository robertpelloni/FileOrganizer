#include "fo/core/provider_registration.hpp"

namespace fo::core {
    void register_all_providers() {
        register_scanner_std();
#ifdef _WIN32
        register_scanner_win32();
#endif
        register_scanner_dirent();
        register_hasher_fast64();
        register_hasher_sha256();
        register_hasher_xxhash();
        register_hasher_blake3();
        register_metadata_tinyexif();
    }
}
