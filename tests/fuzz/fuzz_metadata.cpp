#include <cstdint>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include "fo/core/interfaces.hpp"
#include "fo/core/registry.hpp"
#include "fo/core/provider_registration.hpp"

namespace fs = std::filesystem;

// The main entry point is required if we are not linking against libFuzzer's main.
// Since we are just compiling an executable to ensure it builds, we need a main.
// If we were using clang -fsanitize=fuzzer, it would provide main.

// Helper function to process input (what the fuzzer would call)
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    static bool initialized = []{
        fo::core::register_all_providers();
        return true;
    }();

    // Create a temporary file with the fuzz data
    fs::path temp_path = fs::temp_directory_path() / "fuzz_metadata.tmp";
    
    {
        std::ofstream ofs(temp_path, std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(Data), Size);
    }

    // Create the provider
    auto provider = fo::core::Registry<fo::core::IMetadataProvider>::instance().create("tinyexif");
    if (provider) {
        fo::core::ImageMetadata metadata;
        // The provider reads from the file on disk
        provider->read(temp_path, metadata);
    }

    // Cleanup
    std::error_code ec;
    fs::remove(temp_path, ec);
    
    return 0; 
}

// Minimal main for "seed" testing or just compiling
int main(int argc, char** argv) {
    if (argc > 1) {
        // Read file provided as argument and pass to fuzzer
        std::ifstream f(argv[1], std::ios::binary);
        if (f) {
            std::vector<char> buffer((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            LLVMFuzzerTestOneInput(reinterpret_cast<const uint8_t*>(buffer.data()), buffer.size());
        }
    } else {
        // Dummy run
        const uint8_t dummy[] = "test";
        LLVMFuzzerTestOneInput(dummy, 4);
    }
    return 0;
}
