#pragma once
#include "fo/core/database.hpp"
#include <string>
#include <chrono>

namespace fo::core {

struct ScanSession {
    int64_t id = 0;
    int64_t start_time = 0;
    int64_t end_time = 0;
    std::string status;
    int scanned_count = 0;
    int64_t duration_ms = 0;
};

class ScanSessionRepository {
public:
    explicit ScanSessionRepository(DatabaseManager& db);

    int64_t start_session();
    void end_session(int64_t id, const std::string& status, int scanned_count);

private:
    DatabaseManager& db_;
};

} // namespace fo::core
