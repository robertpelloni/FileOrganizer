#include "fo/providers/blake3/blake3_hasher.hpp"
#include "fo/core/provider.hpp"

#ifdef FO_HAVE_BLAKE3

namespace fo::providers::blake3 {
    struct Blake3Hasher::Impl {
        blake3_hasher hasher;
    };

    Blake3Hasher::Blake3Hasher() : impl_(std::make_unique<Impl>()) {
        blake3_hasher_init(&impl_->hasher);
    }

    Blake3Hasher::~Blake3Hasher() = default;

    IHasher::Hash Blake3Hasher::compute(std::istream& file) const {
        // Placeholder
        return {};
    }
}

namespace {
    [[maybe_unused]] auto reg =
        fo::core::Provider<fo::core::IHasher, fo::providers::blake3::Blake3Hasher>("blake3");
}

#endif
