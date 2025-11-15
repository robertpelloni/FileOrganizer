#include "fo/providers/ocr_tesseract.hpp"

// TODO: Implement Tesseract provider when building with Tesseract support
// Requires: vcpkg install tesseract
// Then: #include <tesseract/baseapi.h>
// Feature-guard with #ifdef FO_HAVE_TESSERACT

namespace fo::providers {

std::optional<fo::core::OCRResult> TesseractOCRProvider::recognize(const std::filesystem::path& image_path, const std::string& lang) {
    (void)image_path; (void)lang;
    // Placeholder: not implemented without Tesseract
    return std::nullopt;
}

std::vector<fo::core::OCRBoundingBox> TesseractOCRProvider::recognize_detailed(const std::filesystem::path& image_path, const std::string& lang) {
    (void)image_path; (void)lang;
    // Placeholder: not implemented without Tesseract
    return {};
}

} // namespace fo::providers
