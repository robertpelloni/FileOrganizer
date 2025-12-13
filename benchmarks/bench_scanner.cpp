#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "fo/core/interfaces.hpp"
#include "fo/core/registry.hpp"
#include "fo/core/provider_registration.hpp"

using steady_clock_t = std::chrono::steady_clock;

static std::vector<std::string> split_csv(const std::string& s) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == ',') { out.push_back(cur); cur.clear(); }
        else { cur.push_back(c); }
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}

static double percentile(const std::vector<double>& v, double p) {
    if (v.empty()) return 0.0;
    std::vector<double> c = v;
    std::sort(c.begin(), c.end());
    double idx = (p/100.0) * (c.size() - 1);
    size_t i = static_cast<size_t>(idx);
    double frac = idx - i;
    if (i + 1 < c.size()) return c[i] * (1.0 - frac) + c[i+1] * frac;
    return c[i];
}

int main(int argc, char** argv) {
    fo::core::register_all_providers();
    std::string scanner_name = "std";
    std::vector<std::filesystem::path> roots;
    std::vector<std::string> exts;
    int iters = 5;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a.rfind("--scanner=", 0) == 0) {
            scanner_name = a.substr(10);
        } else if (a.rfind("--ext=", 0) == 0) {
            exts = split_csv(a.substr(6));
        } else if (a.rfind("--iters=", 0) == 0) {
            iters = std::max(1, std::stoi(a.substr(8)));
        } else if (a == "--help" || a == "-h") {
            std::cout << "Usage: fo_bench_scanner [--scanner=name] [--ext=.jpg,.png] [--iters=N] DIR...\n";
            return 0;
        } else {
            roots.emplace_back(a);
        }
    }

    if (roots.empty()) {
        std::cerr << "Error: provide at least one directory.\n";
        return 1;
    }

    auto& reg = fo::core::Registry<fo::core::IFileScanner>::instance();
    std::unique_ptr<fo::core::IFileScanner> scanner = reg.create(scanner_name);
    if (!scanner) {
        std::cerr << "Unknown scanner: " << scanner_name << "\nAvailable:";
        for (auto& n : reg.names()) std::cerr << " " << n;
        std::cerr << "\n";
        return 2;
    }

    std::cout << "Scanner: " << scanner->name() << "\n";
    std::cout << "Iters: " << iters << "\n";

    std::vector<double> files_per_sec;
    std::uint64_t total_files_last = 0;

    for (int k = 0; k < iters; ++k) {
        auto t0 = steady_clock_t::now();
        auto files = scanner->scan(roots, exts, false);
        auto t1 = steady_clock_t::now();
        std::chrono::duration<double> dt = t1 - t0;
        double secs = dt.count();
        double fps = files.empty() ? 0.0 : (files.size() / secs);
        files_per_sec.push_back(fps);
        total_files_last = static_cast<std::uint64_t>(files.size());
        std::cout << "Run " << (k+1) << ": files=" << files.size() << ", time=" << std::fixed << std::setprecision(3) << secs << "s, throughput=" << std::setprecision(1) << fps << " files/s\n";
    }

    double med = percentile(files_per_sec, 50.0);
    double p90 = percentile(files_per_sec, 90.0);

    std::cout << "Summary: files_last=" << total_files_last
              << ", median_throughput=" << std::fixed << std::setprecision(1) << med
              << " files/s, p90=" << p90 << " files/s\n";

    return 0;
}
