#include "fo/core/rule_engine.hpp"
#include <regex>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace fo::core {

RuleEngine::RuleEngine() {}

void RuleEngine::add_rule(const OrganizationRule& rule) {
    rules_.push_back(rule);
}

std::optional<std::filesystem::path> RuleEngine::apply_rules(const FileInfo& file, const std::vector<std::string>& tags) {
    for (const auto& rule : rules_) {
        // Check filter
        bool match = false;
        if (rule.filter_tag.empty()) {
            match = true;
        } else {
            for (const auto& tag : tags) {
                if (tag == rule.filter_tag) {
                    match = true;
                    break;
                }
            }
        }

        if (match) {
            std::string new_dir_str = expand_template(rule.destination_template, file, tags);
            std::filesystem::path new_dir(new_dir_str);
            return new_dir / file.path.filename();
        }
    }
    return std::nullopt;
}

std::string RuleEngine::expand_template(const std::string& tmpl, const FileInfo& file, const std::vector<std::string>& tags) {
    std::string result = tmpl;
    
    // Extract date
    // Portable conversion from file_time_type to system_clock is messy in C++20 until C++23
    // We use a rough approximation
    auto ftime = file.last_write_time;
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
    );
    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
    std::tm* gmt = std::gmtime(&tt);

    if (gmt) {
        std::stringstream year_ss; year_ss << (gmt->tm_year + 1900);
        std::stringstream month_ss; month_ss << std::setw(2) << std::setfill('0') << (gmt->tm_mon + 1);
        std::stringstream day_ss; day_ss << std::setw(2) << std::setfill('0') << gmt->tm_mday;

        auto replace_all = [&](std::string& str, const std::string& from, const std::string& to) {
            size_t start_pos = 0;
            while((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length();
            }
        };

        replace_all(result, "{year}", year_ss.str());
        replace_all(result, "{month}", month_ss.str());
        replace_all(result, "{day}", day_ss.str());
    }

    auto replace_all = [&](std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    };

    if (file.path.has_extension()) {
        replace_all(result, "{ext}", file.path.extension().string().substr(1));
    }
    replace_all(result, "{name}", file.path.stem().string());

    return result;
}

} // namespace fo::core
