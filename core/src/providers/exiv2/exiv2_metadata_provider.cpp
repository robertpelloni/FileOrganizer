#include "fo/providers/exiv2/exiv2_metadata_provider.hpp"
#include "fo/core/provider.hpp"

#ifdef FO_HAVE_EXIV2

namespace fo::providers::exiv2 {
    struct Exiv2MetadataProvider::Impl {};

    Exiv2MetadataProvider::Exiv2MetadataProvider() : impl_(std::make_unique<Impl>()) {}
    Exiv2MetadataProvider::~Exiv2MetadataProvider() = default;

    bool Exiv2MetadataProvider::read(const std::filesystem::path& p, core::ImageMetadata& out) {
        // Placeholder implementation
        (void)p;
        (void)out;
        return false;
    }
}

namespace {
    [[maybe_unused]] auto reg =
        fo::core::Provider<fo::core::IMetadataProvider,
                           fo::providers::exiv2::Exiv2MetadataProvider>("exiv2");
}

#endif
