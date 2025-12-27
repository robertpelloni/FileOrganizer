#include "fo/core/engine.hpp"
#include "fo/core/registry.hpp"
#include "fo/core/provider_registration.hpp"
#include "fo/core/ocr_interface.hpp"
#include "fo/core/perceptual_hash_interface.hpp"
#include "fo/core/classification_interface.hpp"
#include "fo/core/rule_engine.hpp"
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
              << "  similar      Find similar images\n"
              << "  classify     Classify images using AI\n"
              << "  organize     Organize files based on rules\n"
              << "  delete-duplicates Delete duplicate files\n"
              << "  rename       Rename files based on pattern\n"
              << "\nOptions:\n"
              << "  --scanner=<name>    Select scanner (e.g., std, win32, dirent)\n"
              << "  --hasher=<name>     Select hasher (e.g., fast64, blake3)\n"
              << "  --db=<path>         Database path (default: fo.db)\n"
              << "  --rule=<template>   Organization rule (e.g., '/Photos/{year}/{month}')\n"
              << "  --rules=<file.yaml> Load organization rules from YAML file\n"
              << "  --pattern=<tmpl>    Rename pattern (e.g., '{year}_{name}.{ext}')\n"
              << "  --keep=<strategy>   Keep strategy: oldest, newest, shortest, longest (default: oldest)\n"
              << "  --dry-run           Simulate organization without moving files\n"
              << "  --ext=<.jpg,.png>   Comma-separated list of extensions\n"
              << "  --follow-symlinks   Follow symbolic links\n"
              << "  --format=<json>     Output format\n"
              << "  --threshold=<N>     Similarity threshold (default: 10)\n"
              << "  --list-scanners     List available scanners\n"
              << "  --list-hashers      List available hashers\n"
              << "  --list-metadata     List available metadata providers\n"
              << "  --list-ocr          List available OCR providers\n"
              << "  --list-classifiers  List available classifiers\n"
              << "  --download-models   Download default AI models\n";
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
    if (command == "--download-models") {
        std::cout << "Downloading models...\n";
        // Placeholder for actual download logic
        // In a real scenario, we would use libcurl or a system command
        // For now, just print instructions
        std::cout << "Please download the following files to your working directory:\n";
        std::cout << "1. ResNet50 ONNX model: https://github.com/onnx/models/raw/main/vision/classification/resnet/model/resnet50-v2-7.onnx (rename to model.onnx)\n";
        std::cout << "2. ImageNet Labels: https://raw.githubusercontent.com/onnx/models/main/vision/classification/synset.txt (rename to labels.txt)\n";
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
    std::string rule_template;
    std::string rules_file;
    std::string rename_pattern;
    std::string keep_strategy = "oldest";
    bool dry_run = false;
    int threshold = 10;
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
        else if (a == "--list-classifiers") {
            auto& reg = fo::core::Registry<fo::core::IImageClassifier>::instance();
            std::cout << "Available classifiers:";
            for (auto& n : reg.names()) std::cout << " " << n;
            std::cout << "\n";
            return 0;
        }
        else if (a.rfind("--scanner=", 0) == 0) cfg.scanner = a.substr(10);
        else if (a.rfind("--hasher=", 0) == 0) cfg.hasher = a.substr(9);
        else if (a.rfind("--db=", 0) == 0) cfg.db_path = a.substr(5);
        else if (a.rfind("--rule=", 0) == 0) rule_template = a.substr(7);
        else if (a.rfind("--rules=", 0) == 0) rules_file = a.substr(8);
        else if (a.rfind("--pattern=", 0) == 0) rename_pattern = a.substr(10);
        else if (a.rfind("--keep=", 0) == 0) keep_strategy = a.substr(7);
        else if (a == "--dry-run") dry_run = true;
        else if (a.rfind("--lang=", 0) == 0) lang = a.substr(7);
        else if (a.rfind("--threshold=", 0) == 0) threshold = std::stoi(a.substr(12));
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
        } else if (command == "similar") {
            if (roots.empty()) {
                std::cerr << "Usage: fo similar <image_path> [--threshold=10]\n";
                return 1;
            }
            
            auto provider = fo::core::Registry<fo::core::IPerceptualHasher>::instance().create("opencv");
            if (!provider) {
                std::cerr << "Perceptual hasher 'opencv' not found.\n";
                return 1;
            }
            
            auto res = provider->compute(roots[0]);
            if (!res) {
                std::cerr << "Failed to compute hash for " << roots[0] << "\n";
                return 1;
            }
            
            std::cout << "Target hash: " << res->value << " (" << res->method << ")\n";
            
            auto matches = engine.file_repository().find_similar_images(res->value, threshold);
            std::cout << "Found " << matches.size() << " similar images:\n";
            for (auto id : matches) {
                auto fi = engine.file_repository().get_by_id(id);
                if (fi) {
                    std::cout << "  " << fi->path.string() << "\n";
                }
            }
        } else if (command == "classify") {
            auto files = engine.scan(roots, exts, follow_symlinks);
            auto provider = fo::core::Registry<fo::core::IImageClassifier>::instance().create("onnx");
            if (!provider) {
                std::cerr << "Classifier 'onnx' not found.\n";
                return 1;
            }
            for (const auto& f : files) {
                auto results = provider->classify(f.path);
                if (!results.empty()) {
                    std::cout << f.path.string() << ":\n";
                    for (const auto& r : results) {
                        std::cout << "  " << r.label << " (" << r.confidence << ")\n";
                        // Persist to DB if file is tracked
                        if (f.id != 0) {
                            engine.file_repository().add_tag(f.id, r.label, r.confidence, "ai");
                        }
                    }
                }
            }
        } else if (command == "organize") {
            if (rule_template.empty() && rules_file.empty()) {
                std::cerr << "Error: --rule or --rules argument is required for organize command.\n";
                return 1;
            }

            auto files = engine.scan(roots, exts, follow_symlinks);
            fo::core::RuleEngine rule_engine;
            
            if (!rule_template.empty()) {
                rule_engine.add_rule({"cli_rule", "", rule_template});
            }
            
            if (!rules_file.empty()) {
                if (!rule_engine.load_rules_from_yaml(rules_file)) {
                    std::cerr << "Failed to load rules from " << rules_file << "\n";
                    return 1;
                }
            }

            std::cout << "Organizing " << files.size() << " files...\n";
            if (dry_run) std::cout << "(Dry run - no files will be moved)\n";

            for (const auto& f : files) {
                std::vector<std::string> tags;
                // If we have a DB and the file is tracked, fetch tags
                if (f.id != 0) {
                    auto tag_pairs = engine.file_repository().get_tags(f.id);
                    for (const auto& p : tag_pairs) tags.push_back(p.first);
                }

                auto new_path = rule_engine.apply_rules(f, tags);
                if (new_path != f.path) {
                    std::cout << f.path.string() << " -> " << new_path.string() << "\n";
                    if (!dry_run) {
                        try {
                            std::filesystem::create_directories(new_path.parent_path());
                            std::filesystem::rename(f.path, new_path);
                            // Update DB if tracked
                            if (f.id != 0) {
                                // This is a simplification; ideally we'd update the path in the DB
                                // But FileRepository might not expose a direct update_path method yet
                                // For now, we just move the file on disk.
                            }
                        } catch (const std::exception& e) {
                            std::cerr << "Failed to move " << f.path.string() << ": " << e.what() << "\n";
                        }
                    }
                }
            }
                    }
                }
            }
        } else if (command == "delete-duplicates") {
            auto groups = engine.duplicate_repository().get_all_groups();
            std::cout << "Found " << groups.size() << " duplicate groups.\n";
            if (dry_run) std::cout << "(Dry run - no files will be deleted)\n";

            int deleted_count = 0;
            int kept_count = 0;

            for (const auto& g : groups) {
                std::vector<fo::core::FileInfo> members;
                // Add primary
                auto p = engine.file_repository().get_by_id(g.primary_file_id);
                if (p) members.push_back(*p);
                // Add others
                for (auto mid : g.member_ids) {
                    if (mid == g.primary_file_id) continue; // Avoid double counting if primary is in members
                    auto m = engine.file_repository().get_by_id(mid);
                    if (m) members.push_back(*m);
                }

                if (members.size() < 2) continue;

                // Sort based on strategy
                std::sort(members.begin(), members.end(), [&](const fo::core::FileInfo& a, const fo::core::FileInfo& b) {
                    if (keep_strategy == "newest") return a.last_write_time > b.last_write_time;
                    if (keep_strategy == "shortest") return a.path.string().length() < b.path.string().length();
                    if (keep_strategy == "longest") return a.path.string().length() > b.path.string().length();
                    // Default: oldest
                    return a.last_write_time < b.last_write_time;
                });

                // Keep the first one
                const auto& keep = members[0];
                kept_count++;
                std::cout << "Keeping: " << keep.path.string() << "\n";

                // Delete the rest
                for (size_t i = 1; i < members.size(); ++i) {
                    const auto& del = members[i];
                    std::cout << "  Deleting: " << del.path.string() << "\n";
                    if (!dry_run) {
                        try {
                            std::filesystem::remove(del.path);
                            deleted_count++;
                            // Ideally remove from DB too, but next scan will fix it
                        } catch (const std::exception& e) {
                            std::cerr << "    Failed to delete: " << e.what() << "\n";
                        }
                    }
                }
            }
            std::cout << "Deleted " << deleted_count << " files. Kept " << kept_count << " files.\n";

        } else if (command == "rename") {
            if (rename_pattern.empty()) {
                std::cerr << "Error: --pattern argument is required for rename command.\n";
                return 1;
            }

            // If pattern doesn't start with {parent} or /, prepend {parent}/
            if (rename_pattern.find("{parent}") == std::string::npos && 
                rename_pattern.find("/") == std::string::npos && 
                rename_pattern.find("\\") == std::string::npos) {
                rename_pattern = "{parent}/" + rename_pattern;
            }

            auto files = engine.scan(roots, exts, follow_symlinks);
            fo::core::RuleEngine rule_engine;
            rule_engine.add_rule({"rename_rule", "", rename_pattern});

            std::cout << "Renaming " << files.size() << " files using pattern: " << rename_pattern << "\n";
            if (dry_run) std::cout << "(Dry run - no files will be renamed)\n";

            for (const auto& f : files) {
                std::vector<std::string> tags;
                if (f.id != 0) {
                    auto tag_pairs = engine.file_repository().get_tags(f.id);
                    for (const auto& p : tag_pairs) tags.push_back(p.first);
                }

                auto new_path = rule_engine.apply_rules(f, tags);
                if (new_path && *new_path != f.path) {
                    std::cout << f.path.string() << " -> " << new_path->string() << "\n";
                    if (!dry_run) {
                        try {
                            std::filesystem::rename(f.path, *new_path);
                        } catch (const std::exception& e) {
                            std::cerr << "Failed to rename " << f.path.string() << ": " << e.what() << "\n";
                        }
                    }
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
