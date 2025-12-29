#pragma once

#include "fo/core/interfaces.hpp"

#ifdef FO_HAVE_BLAKE3
#include <blake3.h>
#endif

namespace fo::providers::blake3 {

    class Blake3Hasher : public core::IHasher {
      public:
        Blake3Hasher();
        ~Blake3Hasher();
        std::string fast64(const std::filesystem::path& p) override;
        std::optional<std::string> strong(const std::filesystem::path& p) override;
        std::string strong_algo() const override;
        std::string name() const override {
            return "blake3";
        }

      private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

}
