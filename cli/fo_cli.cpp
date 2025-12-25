#include "fo/core/engine.hpp"
#include "fo/core/registry.hpp"
#include "fo/core/provider_registration.hpp"
#include "fo/core/ocr_interface.hpp"
#include "fo/core/version.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "fo/core/provider_registration.hpp"

using namespace std::chrono;

static void print_usage() {
    std::cout << "FileOrganizer v" << fo::core::FO_VERSION << "\n";
    std::cout << "Usage: fo_cli <command> [options] [paths...]\n"
              << "Commands:\n"
              << "  scan         Scan for files\n"
              << "  duplicates   Find duplicate files\n"
              << "  hash         Compute file hashes\n"
              << "  metadata     Extract file metadata\n"
              << "  ocr          Extract text from images\n"
              << "\nOptions:\n"
              << "  --scanner=<name>    Select scanner (e.g., std, win32, dirent)\n"
              << "  --hasher=<name>     Select hasher (e.g., fast64, blake3)\n"
              << "  --db=<path>         Database path (default: fo.db)\n"
              << "  --ext=<.jpg,.png>   Comma-separated list of extensions\n"
              << "  --follow-symlinks   Follow symbolic links\n"
              << "  --format=<json>     Output format\n"
              << "  --list-scanners     List available scanners\n"
              << "  --list-hashers      List available hashers\n"
              << "  --list-metadata     List available metadata providers\n"
              << "  --list-ocr          List available OCR providers\n";
}

int main(int argc, char** argv) {
    fo::core::register_all_providers();

    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string command = argv[1];
    if (command == "-h" || command == "--help") {
        print_usage();
        return 0;
    }
    if (command == "-v" || command == "--version") {
        std::cout << "FileOrganizer v" << fo::core::FO_VERSION << "\n";
        return 0;
    }

    std::vector<std::filesystem::path> roots;
    std::vector<std::string> exts;
    bool follow_symlinks = false;
    std::string format;
    std::string lang = "eng";
    fo::core::EngineConfig cfg;

    for (int i = 2; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-h" || a == "--help") { print_usage(); return 0; }
        else if (a == "--list-scanners") {
            auto& reg = fo::core::Registry<fo::core::IFileScanner>::instance();
            std::cout << "Available scanners:";
            for (auto& n : reg.names()) std::cout << " " << n;
            std::cout << "\n";
            return 0;
        }
        else if (a == "--list-hashers") {
            auto& reg = fo::core::Registry<fo::core::IHasher>::instance();
            std::cout << "Available hashers:";
            for (auto& n : reg.names()) std::cout << " " << n;
            std::cout << "\n";
            return 0;
        }
        else if (a == "--list-metadata") {
            auto& reg = fo::core::Registry<fo::core::IMetadataProvider>::instance();
            std::cout << "Available metadata providers:";
            for (auto& n : reg.names()) std::cout << " " << n;
            std::cout << "\n";
            return 0;
        }
        else if (a == "--list-ocr") {
            auto& reg = fo::core::Registry<fo::core::IOCRProvider>::instance();
            std::cout << "Available OCR providers:";
            for (auto& n : reg.names()) std::cout << " " << n;
            std::cout << "\n";
            return 0;
        }
        else if (a.rfind("--scanner=", 0) == 0) cfg.scanner = a.substr(10);
        else if (a.rfind("--hasher=", 0) == 0) cfg.hasher = a.substr(9);
        else if (a.rfind("--db=", 0) == 0) cfg.db_path = a.substr(5);
        else if (a.rfind("--lang=", 0) == 0) lang = a.substr(7);
        else if (a.rfind("--ext=", 0) == 0) {
            auto list = a.substr(6);
            size_t pos = 0;
            while (pos < list.size()) {
                auto comma = list.find(',', pos);
                if (comma == std::string::npos) comma = list.size();
                exts.push_back(list.substr(pos, comma - pos));
                pos = comma + 1;
            }
        } else if (a.rfind("--format=", 0) == 0) {
            format = a.substr(9);
        } else if (a == "--follow-symlinks") {
            follow_symlinks = true;
        } else if (!a.empty() && a[0] == '-') {
            std::cerr << "Unknown option: " << a << "\n";
            return 2;
        } else {
            roots.emplace_back(a);
        }
    }

    try {
        fo::core::Engine engine(cfg);

        if (command == "scan") {
            auto files = engine.scan(roots, exts, follow_symlinks);
            for (const auto& f : files) {
                std::cout << f.path.string() << "\n";
            }
        } else if (command == "duplicates") {
            auto files = engine.scan(roots, exts, follow_symlinks);
            auto groups = engine.find_duplicates(files);
            for (const auto& g : groups) {
                std::cout << "== size=" << g.size << ", fast64=" << g.fast64 << "\n";
                for (const auto& f : g.files) {
                    std::cout << "  " << f.path.string() << "\n";
                }
            }
        } else if (command == "hash") {
            auto files = engine.scan(roots, exts, follow_symlinks);
            auto& hasher = engine.hasher();
            for (const auto& f : files) {
                std::string h = hasher.fast64(f.path);
                std::cout << h << "  " << f.path.string() << "\n";
                // Optionally persist hash
                if (f.id != 0) {
                    engine.file_repository().add_hash(f.id, hasher.name(), h);
                }
            }
        } else if (command == "metadata") {
            auto files = engine.scan(roots, exts, follow_symlinks);
            // Use default metadata provider for now, or add CLI option
            auto provider = fo::core::Registry<fo::core::IMetadataProvider>::instance().create("tinyexif");
            if (!provider) {
                std::cerr << "Metadata provider 'tinyexif' not found.\n";
                return 1;
            }
            for (const auto& f : files) {
                fo::core::ImageMetadata meta;
                if (provider->read(f.path, meta)) {
                    std::cout << f.path.string() << ":\n";
                    if (meta.date.has_taken) {
                        auto t = std::chrono::system_clock::to_time_t(meta.date.taken);
                        std::cout << "  Taken: " << std::ctime(&t); // ctime includes newline
                    }
                    if (meta.has_gps) {
                        std::cout << "  GPS: " << meta.gps_lat << ", " << meta.gps_lon << "\n";
                    }
                }
            }
        } else if (command == "ocr") {
            auto files = engine.scan(roots, exts, follow_symlinks);
            auto provider = fo::core::Registry<fo::core::IOCRProvider>::instance().create("tesseract");
            if (!provider) {
                std::cerr << "OCR provider 'tesseract' not found.\n";
                return 1;
            }
            for (const auto& f : files) {
                auto result = provider->recognize(f.path, lang);
                if (result) {
                    std::cout << f.path.string() << ":\n";
                    std::cout << "  Text: " << result->text << "\n";
                }
            }
        } else {
            std::cerr << "Unknown command: " << command << "\n";
            return 1;
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 3;
    }
}
