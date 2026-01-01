#include "fo/core/lint_interface.hpp"
#include "fo/core/registry.hpp"
#include <filesystem>
#include <regex>

namespace fo::core {

class StdLinter : public ILinter {
public:
    std::string name() const override { return "std"; }

    std::vector<LintResult> lint(const std::vector<std::filesystem::path>& roots) override {
        std::vector<LintResult> results;
        std::regex temp_regex(R"(^(\.tmp|~\$|\.bak|.*\.tmp|.*\.swp)$)", std::regex::icase);

        for (const auto& root : roots) {
            if (!std::filesystem::exists(root)) continue;

            for (auto it = std::filesystem::recursive_directory_iterator(root, std::filesystem::directory_options::skip_permission_denied);
                 it != std::filesystem::recursive_directory_iterator(); ++it) {
                
                std::error_code ec;
                const auto& entry = *it;
                const auto& path = entry.path();

                // 1. Broken Symlinks
                if (entry.is_symlink()) {
                    if (!std::filesystem::exists(std::filesystem::read_symlink(path, ec))) { // Logic check: read_symlink gives target, exists checks it
                         // Actually, std::filesystem::exists(path) on a symlink checks if the target exists.
                         // But we need to be careful about cyclic or weird links.
                         // Simplest check:
                         if (!std::filesystem::exists(path)) {
                             results.push_back({path, LintType::BrokenSymlink, "Target does not exist"});
                         }
                    }
                    continue; 
                }

                // 2. Empty Files
                if (entry.is_regular_file()) {
                    if (entry.file_size(ec) == 0 && !ec) {
                        results.push_back({path, LintType::EmptyFile, "File is empty"});
                    }
                    // 3. Temporary Files (by extension/pattern)
                    std::string filename = path.filename().string();
                    std::string ext = path.extension().string();
                    
                    if (std::regex_match(ext, temp_regex) || filename.starts_with("~$")) {
                         results.push_back({path, LintType::TemporaryFile, "Matches temporary file pattern"});
                    }
                }

                // 4. Empty Directories
                // Note: recursive_iterator visits directories.
                if (entry.is_directory()) {
                    if (std::filesystem::is_empty(path, ec) && !ec) {
                        results.push_back({path, LintType::EmptyDirectory, "Directory is empty"});
                    }
                }
            }
        }
        return results;
    }
};

// Static registration
static bool reg_linter_std = [](){
    Registry<ILinter>::instance().add("std", [](){ return std::make_unique<StdLinter>(); });
    return true;
}();

void register_linter_std() { (void)reg_linter_std; }

} // namespace fo::core
