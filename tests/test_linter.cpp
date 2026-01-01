#include <gtest/gtest.h>
#include "fo/core/lint_interface.hpp"
#include "fo/core/registry.hpp"
#include "fo/core/provider_registration.hpp"
#include <fstream>
#include <filesystem>

using namespace fo::core;

class StdLinterTest : public ::testing::Test {
protected:
    void SetUp() override {
        register_all_providers();
        
        test_dir = std::filesystem::temp_directory_path() / "fo_test_linter";
        std::filesystem::create_directories(test_dir);
    }
    
    void TearDown() override {
        std::filesystem::remove_all(test_dir);
    }
    
    std::filesystem::path test_dir;
};

TEST_F(StdLinterTest, LinterExists) {
    auto linter = Registry<ILinter>::instance().create("std");
    ASSERT_NE(linter, nullptr);
    EXPECT_EQ(linter->name(), "std");
}

TEST_F(StdLinterTest, DetectsEmptyFile) {
    auto linter = Registry<ILinter>::instance().create("std");
    auto empty_file = test_dir / "empty.txt";
    std::ofstream(empty_file).close(); // Create empty file
    
    auto results = linter->lint({test_dir});
    
    bool found = false;
    for (const auto& r : results) {
        if (r.path == empty_file && r.type == LintType::EmptyFile) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST_F(StdLinterTest, DetectsEmptyDirectory) {
    auto linter = Registry<ILinter>::instance().create("std");
    auto empty_subdir = test_dir / "empty_subdir";
    std::filesystem::create_directories(empty_subdir);
    
    auto results = linter->lint({test_dir});
    
    bool found = false;
    for (const auto& r : results) {
        if (r.path == empty_subdir && r.type == LintType::EmptyDirectory) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST_F(StdLinterTest, DetectsTempFile) {
    auto linter = Registry<ILinter>::instance().create("std");
    auto temp_file = test_dir / "doc.bak";
    std::ofstream(temp_file) << "content"; 
    
    auto results = linter->lint({test_dir});
    
    bool found = false;
    for (const auto& r : results) {
        if (r.path == temp_file && r.type == LintType::TemporaryFile) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}
