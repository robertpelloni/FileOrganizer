#include "fo/core/provider_registration.hpp"
#include "fo/providers/dhash.hpp"
#include "fo/core/provider.hpp"
#include "fo/core/registry.hpp"

namespace fo::core {
    void register_hasher_dhash() {
        [[maybe_unused]] static const auto reg =
            Provider<IHasher, fo::providers::DHash>("dhash");
    }
}
