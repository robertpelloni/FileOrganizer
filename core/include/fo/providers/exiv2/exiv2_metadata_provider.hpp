#pragma once

#include "fo/core/interfaces.hpp"

#ifdef FO_HAVE_EXIV2
#include <exiv2/exiv2.hpp>
#endif

namespace fo::providers::exiv2 {
    class Exiv2MetadataProvider : public core::IMetadataProvider {
      public:
        Exiv2MetadataProvider();
        ~Exiv2MetadataProvider();

        bool read(const std::filesystem::path& p, core::ImageMetadata& out) override;

      private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
