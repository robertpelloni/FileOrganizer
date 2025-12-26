#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <optional>

namespace fo::core {

struct ClassificationResult {
    std::string label;
    float confidence;
};

class IImageClassifier {
public:
    virtual ~IImageClassifier() = default;
    virtual std::string name() const = 0;
    
    // Classify an image, returning top-k results
    virtual std::vector<ClassificationResult> classify(
        const std::filesystem::path& image_path, 
        int top_k = 3) = 0;
};

} // namespace fo::core
