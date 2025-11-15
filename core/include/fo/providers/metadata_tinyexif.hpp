#pragma once

#include "fo/core/interfaces.hpp"

namespace fo::providers {

class TinyExifMetadataProvider : public fo::core::IMetadataProvider {
public:
    std::string name() const override { return "tinyexif"; }
    bool read(const std::filesystem::path& p, fo::core::ImageMetadata& out) override;
    // TinyEXIF is read-only; write not supported
};

} // namespace fo::providers
