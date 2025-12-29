#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include "fo/core/interfaces.hpp"

// Forward declare or include the scanner implementation you want to fuzz.
// For example, if testing std scanner:
#include "fo/core/registry.hpp"
#include "fo/core/provider_registration.hpp"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    static bool initialized = []{
        fo::core::register_all_providers();
        return true;
    }();

    // Interpret input as file path or content? 
    // Since IFileScanner scans directories, fuzzing it directly without mocking the filesystem is hard.
    // Instead, let's fuzz the Hasher or Metadata parsers which take file content.

    // Let's fuzz the "dHash" strong hash if available, or just a generic provider.
    // Ideally we'd fuzz the MetadataProvider::read method with memory streams.
    
    // Convert fuzz data to a memory stream
    std::string fuzz_data(reinterpret_cast<const char*>(Data), Size);
    
    // For now, let's just ensure it compiles and links. 
    // Real fuzzing would require mocking a file or using a parser that accepts a buffer/stream.
    
    return 0;
}
