#pragma once

#include "fo/core/ocr_interface.hpp"

namespace fo::providers {

// Tesseract OCR provider (requires libtesseract via vcpkg or system install)
// vcpkg: `vcpkg install tesseract`
class TesseractOCRProvider : public fo::core::IOCRProvider {
public:
    std::string name() const override { return "tesseract"; }
    std::optional<fo::core::OCRResult> recognize(const std::filesystem::path& image_path, const std::string& lang = "eng") override;
    std::vector<fo::core::OCRBoundingBox> recognize_detailed(const std::filesystem::path& image_path, const std::string& lang = "eng") override;
};

} // namespace fo::providers
