#pragma once

#include "fo/core/metadata_provider.hpp"

#ifdef FO_HAVE_EXIV2
#include <exiv2/exiv2.hpp>
#endif

namespace fo::providers::exiv2 {
    class Exiv2MetadataProvider : public core::IMetadataProvider {
      public:
        Exiv2MetadataProvider();
        ~Exiv2MetadataProvider();

        std::vector<std::string> supported_extensions() const override;

        core::FileMetadata
        get_metadata(std::istream& file,
                     const std::filesystem::path& file_path) const override;

      private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
}
