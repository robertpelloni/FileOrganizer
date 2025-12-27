#pragma once

#include "interfaces.hpp"
#include "registry.hpp"
#include "database.hpp"
#include "file_repository.hpp"
#include "duplicate_repository.hpp"
#include "ignore_repository.hpp"
#include "scan_session_repository.hpp"
#include <memory>

namespace fo::core {

struct EngineConfig {
    std::string scanner = "std";
    std::string hasher = "fast64";
    std::string db_path = "fo.db";
};

class Engine {
public:
    explicit Engine(EngineConfig cfg = {})
        : cfg_(std::move(cfg))
        , scanner_(Registry<IFileScanner>::instance().create(cfg_.scanner))
        , hasher_(Registry<IHasher>::instance().create(cfg_.hasher))
        , file_repo_(db_manager_)
        , duplicate_repo_(db_manager_)
        , ignore_repo_(db_manager_)
        , session_repo_(db_manager_)
    {
        db_manager_.open(cfg_.db_path);
        db_manager_.migrate();
    }

    std::vector<FileInfo> scan(const std::vector<std::filesystem::path>& roots,
                               const std::vector<std::string>& include_exts,
                               bool follow_symlinks,
                               bool prune = false) {
        if (!scanner_) throw std::runtime_error("scanner not found: " + cfg_.scanner);
        
        int64_t session_id = session_repo_.start_session();
        std::vector<FileInfo> files;
        std::vector<int64_t> scanned_ids;
        
        try {
            files = scanner_->scan(roots, include_exts, follow_symlinks);
            
            // Filter ignored files
            auto ignore_rules = ignore_repo_.get_all();
            if (!ignore_rules.empty()) {
                std::erase_if(files, [&](const FileInfo& f) {
                    return ignore_repo_.is_ignored(f.path.string());
                });
            }

            // Persist to DB
            db_manager_.execute("BEGIN TRANSACTION;");
            for (auto& f : files) {
                auto res = file_repo_.upsert(f);
                if (f.id != 0) scanned_ids.push_back(f.id);
            }
            
            if (prune) {
                file_repo_.prune_missing(scanned_ids, roots);
            }

            db_manager_.execute("COMMIT;");
            
            session_repo_.end_session(session_id, "completed", static_cast<int>(files.size()));
        } catch (...) {
            session_repo_.end_session(session_id, "failed", 0);
            throw;
        }
        
        return files;
    }

    std::vector<DuplicateGroup> find_duplicates(const std::vector<FileInfo>& files) {
        if (!hasher_) throw std::runtime_error("hasher not found: " + cfg_.hasher);
        // use size+fast64 strategy for now
        class SizeHashDuplicateFinder local;
        auto groups = local.group(files, *hasher_);

        // Persist duplicates
        db_manager_.execute("BEGIN TRANSACTION;");
        try {
            duplicate_repo_.clear_all();
            for (auto& g : groups) {
                if (g.files.empty()) continue;
                // Use first file as primary for now
                int64_t primary_id = g.files[0].id;
                // Ensure primary_id is valid (it should be if scan ran)
                if (primary_id == 0) continue; 

                int64_t gid = duplicate_repo_.create_group(primary_id);
                for (auto& f : g.files) {
                    if (f.id != 0) {
                        duplicate_repo_.add_member(gid, f.id);
                    }
                }
            }
            db_manager_.execute("COMMIT;");
        } catch (...) {
            db_manager_.execute("ROLLBACK;");
            throw;
        }

        return groups;
    }

    IHasher& hasher() { return *hasher_; }
    FileRepository& file_repository() { return file_repo_; }
    DuplicateRepository& duplicate_repository() { return duplicate_repo_; }
    IgnoreRepository& ignore_repository() { return ignore_repo_; }
    ScanSessionRepository& session_repository() { return session_repo_; }
    DatabaseManager& database() { return db_manager_; }

private:
    // local implementation of duplicate finder from dupe_size_fast.cpp
    class SizeHashDuplicateFinder : public IDuplicateFinder {
    public:
        std::string name() const override { return "size+fast64"; }
        std::vector<DuplicateGroup> group(const std::vector<FileInfo>& files, IHasher& hasher) override;
    };

    EngineConfig cfg_{};
    std::unique_ptr<IFileScanner> scanner_{};
    std::unique_ptr<IHasher> hasher_{};
    DatabaseManager db_manager_;
    FileRepository file_repo_;
    DuplicateRepository duplicate_repo_;
    IgnoreRepository ignore_repo_;
    ScanSessionRepository session_repo_;
};

} // namespace fo::core
