#pragma once

#include "fo/core/registry.hpp"

namespace fo::core {

    /// A helper class to register a provider with the `Registry`.
    ///
    /// This class is designed to be used as a static variable within a function.
    /// The constructor registers the provider, and the destructor unregisters it.
    ///
    /// \tparam I The interface class (e.g., `IHasher`).
    /// \tparam T The implementation class (e.g., `DHash`).
    template <typename I, typename T>
    struct Provider {
        Provider(const char* name) {
            Registry<I>::instance().add(name, []() { return std::make_unique<T>(); });
        }
    };

}  // namespace fo::core
