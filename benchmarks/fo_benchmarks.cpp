#include <benchmark/benchmark.h>
#include "fo/core/registry.hpp"
#include "fo/core/interfaces.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

class ScannerFixture : public benchmark::Fixture {
public:
    fs::path test_dir;

    void SetUp(const ::benchmark::State&) override {
        test_dir = fs::temp_directory_path() / "fo_bench_scanner";
        if (fs::exists(test_dir)) fs::remove_all(test_dir);
        fs::create_directories(test_dir);
        
        // Create 100 files in root
        for (int i = 0; i < 100; ++i) {
            std::ofstream(test_dir / ("file_" + std::to_string(i) + ".txt")) << "content";
        }
        // Create 10 subdirs with 10 files each
        for (int d = 0; d < 10; ++d) {
            fs::path sub = test_dir / ("subdir_" + std::to_string(d));
            fs::create_directories(sub);
            for (int i = 0; i < 10; ++i) {
                std::ofstream(sub / ("file_" + std::to_string(i) + ".txt")) << "content";
            }
        }
    }

    void TearDown(const ::benchmark::State&) override {
        fs::remove_all(test_dir);
    }
};

BENCHMARK_DEFINE_F(ScannerFixture, ScanStd)(benchmark::State& state) {
    auto scanner = fo::core::Registry<fo::core::IFileScanner>::instance().create("std");
    if (!scanner) { state.SkipWithError("std scanner not found"); return; }
    
    std::vector<fs::path> roots = {test_dir};
    std::vector<std::string> exts; // all
    
    for (auto _ : state) {
        auto files = scanner->scan(roots, exts, false);
        benchmark::DoNotOptimize(files);
    }
}
BENCHMARK_REGISTER_F(ScannerFixture, ScanStd);

#ifdef _WIN32
BENCHMARK_DEFINE_F(ScannerFixture, ScanWin32)(benchmark::State& state) {
    auto scanner = fo::core::Registry<fo::core::IFileScanner>::instance().create("win32");
    if (!scanner) { state.SkipWithError("win32 scanner not found"); return; }
    
    std::vector<fs::path> roots = {test_dir};
    std::vector<std::string> exts; // all
    
    for (auto _ : state) {
        auto files = scanner->scan(roots, exts, false);
        benchmark::DoNotOptimize(files);
    }
}
BENCHMARK_REGISTER_F(ScannerFixture, ScanWin32);
#endif

static void BM_Hasher_Fast64(benchmark::State& state) {
    fs::path path = fs::temp_directory_path() / "fo_bench_hash.tmp";
    {
        std::ofstream ofs(path, std::ios::binary);
        std::vector<char> data(1024 * 1024, 'a'); // 1MB
        ofs.write(data.data(), data.size());
    }
    
    auto hasher = fo::core::Registry<fo::core::IHasher>::instance().create("default");
    if (!hasher) {
        state.SkipWithError("default hasher not found");
        fs::remove(path);
        return;
    }

    for (auto _ : state) {
        std::string h = hasher->fast64(path);
        benchmark::DoNotOptimize(h);
    }
    state.SetBytesProcessed(state.iterations() * 1024 * 1024);
    fs::remove(path);
}
BENCHMARK(BM_Hasher_Fast64);

static void BM_Hasher_Blake3(benchmark::State& state) {
    fs::path path = fs::temp_directory_path() / "fo_bench_hash_b3.tmp";
    {
        std::ofstream ofs(path, std::ios::binary);
        std::vector<char> data(1024 * 1024, 'a'); // 1MB
        ofs.write(data.data(), data.size());
    }
    
    auto hasher = fo::core::Registry<fo::core::IHasher>::instance().create("blake3");
    if (!hasher) {
        state.SkipWithError("blake3 hasher not found");
        fs::remove(path);
        return;
    }

    for (auto _ : state) {
        auto h = hasher->strong(path);
        benchmark::DoNotOptimize(h);
    }
    state.SetBytesProcessed(state.iterations() * 1024 * 1024);
    fs::remove(path);
}
BENCHMARK(BM_Hasher_Blake3);

BENCHMARK_MAIN();
