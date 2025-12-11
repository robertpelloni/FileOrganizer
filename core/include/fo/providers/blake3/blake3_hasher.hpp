#pragma once

#include "fo/core/hasher.hpp"

#ifdef FO_HAVE_BLAKE3
#include <blake3.h>
#endif

namespace fo::providers::blake3 {

    class Blake3Hasher : public core::IHasher {
      public:
        Blake3Hasher();
        ~Blake3Hasher();
        Hash compute(std::istream& file) const override;
        std::string name() const override {
            return "blake3";
        }

      private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

}
