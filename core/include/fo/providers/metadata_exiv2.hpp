#pragma once

#include "fo/core/interfaces.hpp"

namespace fo::providers {

// Exiv2-based metadata provider: read/write EXIF, IPTC, XMP, ICC
// Requires linking with Exiv2 (via vcpkg: `vcpkg install exiv2`)
class Exiv2MetadataProvider : public fo::core::IMetadataProvider {
public:
    std::string name() const override { return "exiv2"; }
    bool read(const std::filesystem::path& p, fo::core::ImageMetadata& out) override;
    bool write(const std::filesystem::path& p, const fo::core::ImageMetadata& in) override;
};

} // namespace fo::providers
