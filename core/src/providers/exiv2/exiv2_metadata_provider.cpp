#include "fo/providers/exiv2/exiv2_metadata_provider.hpp"
#include "fo/core/provider.hpp"

#ifdef FO_HAVE_EXIV2

namespace fo::providers::exiv2 {
    struct Exiv2MetadataProvider::Impl {};

    Exiv2MetadataProvider::Exiv2MetadataProvider() : impl_(std::make_unique<Impl>()) {}
    Exiv2MetadataProvider::~Exiv2MetadataProvider() = default;

    std::vector<std::string> Exiv2MetadataProvider::supported_extensions() const {
        return {".jpg", ".jpeg", ".png", ".gif", ".tiff"};
    }

    core::FileMetadata
    Exiv2MetadataProvider::get_metadata(std::istream& file,
                                          const std::filesystem::path& file_path) const {
        core::FileMetadata md;
        // Placeholder implementation
        return md;
    }
}

namespace {
    [[maybe_unused]] auto reg =
        fo::core::Provider<fo::core::IMetadataProvider,
                           fo::providers::exiv2::Exiv2MetadataProvider>("exiv2");
}

#endif
