#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "fo/core/rule_engine.hpp"
#include "fo/core/types.hpp"

namespace fs = std::filesystem;

// A simplified version of FuzzedDataProvider for our needs
class SimpleFuzzedDataProvider {
public:
    SimpleFuzzedDataProvider(const uint8_t* data, size_t size)
        : data_(data), size_(size), ptr_(0) {}

    std::string ConsumeString(size_t max_length) {
        if (ptr_ >= size_) return "";
        size_t available = size_ - ptr_;
        size_t len = std::min(available, max_length);
        
        // If we have enough data, read a length byte first to vary string sizes
        if (available > 1) {
            uint8_t len_byte = data_[ptr_++];
            len = std::min(static_cast<size_t>(len_byte), len);
            // Ensure we don't go out of bounds after consuming len_byte
            len = std::min(len, size_ - ptr_);
        }
        
        std::string s(reinterpret_cast<const char*>(data_ + ptr_), len);
        ptr_ += len;
        return s;
    }

    std::string ConsumeRemainingBytesAsString() {
        if (ptr_ >= size_) return "";
        std::string s(reinterpret_cast<const char*>(data_ + ptr_), size_ - ptr_);
        ptr_ = size_;
        return s;
    }

    template <typename T>
    T ConsumeIntegral() {
        if (ptr_ + sizeof(T) > size_) return T{};
        T val = *reinterpret_cast<const T*>(data_ + ptr_);
        ptr_ += sizeof(T);
        return val;
    }
    
    // consume a bool
    bool ConsumeBool() {
        if (ptr_ >= size_) return false;
        return (data_[ptr_++] % 2) == 1;
    }

private:
    const uint8_t* data_;
    size_t size_;
    size_t ptr_;
};

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    SimpleFuzzedDataProvider fdp(Data, Size);

    // 1. Construct an OrganizationRule from fuzz data
    fo::core::OrganizationRule rule;
    rule.name = fdp.ConsumeString(32);
    rule.filter_tag = fdp.ConsumeString(16);
    // Give more space for template as it's the complex part
    rule.destination_template = fdp.ConsumeString(128); 

    // 2. Setup the engine
    fo::core::RuleEngine engine;
    engine.add_rule(rule);

    // 3. Construct FileInfo from fuzz data
    fo::core::FileInfo file_info;
    file_info.path = fdp.ConsumeString(64); // Simulate a path
    
    // 4. Construct tags
    std::vector<std::string> tags;
    // Consume a few tags if possible
    while (fdp.ConsumeBool() && tags.size() < 10) {
        tags.push_back(fdp.ConsumeString(16));
    }
    // Add the filter tag sometimes to ensure matches happen
    if (fdp.ConsumeBool()) {
        tags.push_back(rule.filter_tag);
    }

    // 5. Apply rules
    // This exercises the template parsing and substitution logic
    engine.apply_rules(file_info, tags);

    return 0;
}

// Minimal main for non-fuzzer builds
int main(int argc, char** argv) {
    if (argc > 1) {
        // Just run with a dummy input based on args for basic sanity check
        std::string arg = argv[1];
        LLVMFuzzerTestOneInput(reinterpret_cast<const uint8_t*>(arg.data()), arg.size());
    } else {
        // Dummy run with empty data
        const uint8_t dummy[] = "test_rule_engine_fuzzing";
        LLVMFuzzerTestOneInput(dummy, sizeof(dummy));
    }
    return 0;
}
