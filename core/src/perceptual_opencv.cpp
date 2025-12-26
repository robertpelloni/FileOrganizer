#include "fo/core/perceptual_hash_interface.hpp"
#include "fo/core/registry.hpp"

#ifdef FO_HAVE_OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#endif

#include <iostream>

namespace fo::core {

#ifdef FO_HAVE_OPENCV

class OpencvPerceptualHasher : public IPerceptualHasher {
public:
    std::string name() const override { return "opencv"; }

    std::optional<PerceptualHash> compute(const std::filesystem::path& image_path) override {
        try {
            // Load image as grayscale
            cv::Mat img = cv::imread(image_path.string(), cv::IMREAD_GRAYSCALE);
            if (img.empty()) {
                return std::nullopt;
            }

            // dHash implementation
            // 1. Resize to 9x8 (9 columns, 8 rows)
            cv::Mat resized;
            cv::resize(img, resized, cv::Size(9, 8), 0, 0, cv::INTER_AREA);

            // 2. Compute hash
            uint64_t hash_val = 0;
            // We have 8 rows and 8 comparisons per row = 64 bits
            for (int r = 0; r < 8; ++r) {
                const uint8_t* row_ptr = resized.ptr<uint8_t>(r);
                for (int c = 0; c < 8; ++c) {
                    if (row_ptr[c] > row_ptr[c + 1]) {
                        hash_val |= (1ULL << ((r * 8) + c));
                    }
                }
            }

            return PerceptualHash{hash_val, "dhash"};

        } catch (const std::exception& e) {
            std::cerr << "OpenCV error: " << e.what() << std::endl;
            return std::nullopt;
        }
    }
};

namespace {
    // Register the provider
    struct OpencvRegistrar {
        OpencvRegistrar() {
            Registry<IPerceptualHasher>::instance().add("opencv", []() {
                return std::make_unique<OpencvPerceptualHasher>();
            });
        }
    };
    static OpencvRegistrar registrar;
}

#endif // FO_HAVE_OPENCV

} // namespace fo::core
