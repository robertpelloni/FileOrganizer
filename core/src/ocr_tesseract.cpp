#include "fo/providers/ocr_tesseract.hpp"
#include <iostream>

#ifdef FO_HAVE_TESSERACT
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#endif

namespace fo::providers {

std::optional<fo::core::OCRResult> TesseractOCRProvider::recognize(const std::filesystem::path& image_path, const std::string& lang) {
#ifdef FO_HAVE_TESSERACT
    tesseract::TessBaseAPI api;
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api.Init(NULL, lang.c_str())) {
        std::cerr << "Could not initialize tesseract.\n";
        return std::nullopt;
    }

    // Open input image with leptonica library
    Pix *image = pixRead(image_path.string().c_str());
    if (!image) {
        std::cerr << "Could not read image: " << image_path << "\n";
        return std::nullopt;
    }

    api.SetImage(image);
    
    // Get OCR result
    char* outText = api.GetUTF8Text();
    
    fo::core::OCRResult result;
    if (outText) {
        result.text = std::string(outText);
        result.confidence = api.MeanTextConf() / 100.0f;
        result.lang = lang;
        delete [] outText;
    }

    pixDestroy(&image);
    api.End();

    return result;
#else
    (void)image_path; (void)lang;
    std::cerr << "Tesseract support not compiled in. Please install tesseract via vcpkg and rebuild.\n";
    return std::nullopt;
#endif
}

std::vector<fo::core::OCRBoundingBox> TesseractOCRProvider::recognize_detailed(const std::filesystem::path& image_path, const std::string& lang) {
#ifdef FO_HAVE_TESSERACT
    // TODO: Implement detailed bounding box extraction
    (void)image_path; (void)lang;
    return {};
#else
    (void)image_path; (void)lang;
    return {};
#endif
}

} // namespace fo::providers
