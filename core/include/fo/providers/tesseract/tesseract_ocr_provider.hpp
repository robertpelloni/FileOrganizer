#pragma once

#include "fo/core/ocr_provider.hpp"

#ifdef FO_HAVE_TESSERACT
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#endif

namespace fo::providers::tesseract {

    class TesseractOCRProvider : public core::IOCRProvider {
      public:
        TesseractOCRProvider();
        ~TesseractOCRProvider();
        std::string
        extract_text(std::istream& file,
                     const std::filesystem::path& file_path) const override;
        std::vector<std::string> supported_extensions() const override;

      private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

}
