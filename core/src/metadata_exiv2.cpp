#include "fo/providers/metadata_exiv2.hpp"

// TODO: Implement Exiv2 provider when building with Exiv2 support
// Requires: vcpkg install exiv2
// Then: #include <exiv2/exiv2.hpp>
// Feature-guard with #ifdef FO_HAVE_EXIV2

namespace fo::providers {

bool Exiv2MetadataProvider::read(const std::filesystem::path& p, fo::core::ImageMetadata& out) {
    (void)p; (void)out;
    // Placeholder: not implemented without Exiv2
    return false;
}

bool Exiv2MetadataProvider::write(const std::filesystem::path& p, const fo::core::ImageMetadata& in) {
    (void)p; (void)in;
    // Placeholder: not implemented without Exiv2
    return false;
}

} // namespace fo::providers
