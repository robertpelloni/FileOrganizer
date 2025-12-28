#include "fo/core/provider_registration.hpp"

namespace fo::core {
    // Forward declarations for individual provider registration functions.
    void register_hasher_dhash();

    void register_all_providers() {
        register_hasher_dhash();
        // Add calls to other provider registration functions here.
    }
}
