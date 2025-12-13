#include "fo/providers/tesseract/tesseract_ocr_provider.hpp"
#include "fo/core/provider.hpp"

#ifdef FO_HAVE_TESSERACT

namespace fo::providers::tesseract {
    struct TesseractOCRProvider::Impl {
        tesseract::TessBaseAPI api;
    };

    TesseractOCRProvider::TesseractOCRProvider() : impl_(std::make_unique<Impl>()) {
        // Placeholder for initialization
    }

    TesseractOCRProvider::~TesseractOCRProvider() = default;

    std::string
    TesseractOCRProvider::extract_text(std::istream& file,
                                      const std::filesystem::path& file_path) const {
        // Placeholder
        return "";
    }

    std::vector<std::string> TesseractOCRProvider::supported_extensions() const {
        return {".jpg", ".jpeg", ".png", ".tiff"};
    }
}

namespace {
    [[maybe_unused]] auto reg =
        fo::core::Provider<fo::core::IOCRProvider,
                           fo::providers::tesseract::TesseractOCRProvider>("tesseract");
}

#endif
