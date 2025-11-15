#include "fo/core/engine.hpp"
#include "fo/core/registry.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std::chrono;

static void print_usage() {
    std::cout << "Usage: fo_cli [--scanner=std|win32|dirent] [--hasher=fast64] [--ext=.jpg,.png] [--follow-symlinks] [--list-scanners] DIR...\n";
}

int main(int argc, char** argv) {
    std::vector<std::filesystem::path> roots;
    std::vector<std::string> exts;
    bool follow_symlinks = false;
    fo::core::EngineConfig cfg;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-h" || a == "--help") { print_usage(); return 0; }
        else if (a == "--list-scanners") {
            auto& reg = fo::core::Registry<fo::core::IFileScanner>::instance();
            std::cout << "Available scanners:";
            for (auto& n : reg.names()) std::cout << " " << n;
            std::cout << "\n";
            return 0;
        }
        else if (a.rfind("--scanner=", 0) == 0) cfg.scanner = a.substr(10);
        else if (a.rfind("--hasher=", 0) == 0) cfg.hasher = a.substr(9);
        else if (a.rfind("--ext=", 0) == 0) {
            auto list = a.substr(6);
            size_t pos = 0;
            while (pos < list.size()) {
                auto comma = list.find(',', pos);
                if (comma == std::string::npos) comma = list.size();
                exts.push_back(list.substr(pos, comma - pos));
                pos = comma + 1;
            }
        } else if (a == "--follow-symlinks") follow_symlinks = true;
        else if (!a.empty() && a[0] == '-') { std::cerr << "Unknown option: " << a << "\n"; return 2; }
        else roots.emplace_back(a);
    }

    if (roots.empty()) { print_usage(); return 1; }

    try {
        fo::core::Engine engine(cfg);
        auto t0 = steady_clock::now();
        auto files = engine.scan(roots, exts, follow_symlinks);
        auto t1 = steady_clock::now();
        auto groups = engine.find_duplicates(files);
        auto t2 = steady_clock::now();

        std::cout << "Scanned files: " << files.size() << ", time: "
                  << duration_cast<milliseconds>(t1 - t0).count() << " ms\n";
        std::cout << "Duplicate groups (size>=2): " << groups.size() << ", time: "
                  << duration_cast<milliseconds>(t2 - t1).count() << " ms\n";

        for (const auto& g : groups) {
            std::cout << "== size=" << g.size << ", fast64=" << g.fast64 << "\n";
            for (const auto& f : g.files) {
                std::cout << "  " << f.path.string() << "\n";
            }
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 3;
    }
}
