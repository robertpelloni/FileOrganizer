#pragma once
#include "fo/core/database.hpp"
#include <vector>

namespace fo::core {

struct DuplicateGroupDB {
    int64_t id;
    int64_t primary_file_id;
    std::vector<int64_t> member_ids;
};

class DuplicateRepository {
public:
    explicit DuplicateRepository(DatabaseManager& db);

    int64_t create_group(int64_t primary_file_id);
    void add_member(int64_t group_id, int64_t file_id);
    void clear_all(); // Clear all groups (e.g. before a new scan)

    std::vector<DuplicateGroupDB> get_all_groups();

private:
    DatabaseManager& db_;
};

} // namespace fo::core
