#include <gtest/gtest.h>
#include "fo/core/registry.hpp"
#include "fo/core/interfaces.hpp"
#include "fo/core/provider_registration.hpp"
#include <fstream>
#include <filesystem>

using namespace fo::core;

class HasherTest : public ::testing::Test {
protected:
    void SetUp() override {
        register_all_providers();
        
        // Create a temporary test file
        test_file = std::filesystem::temp_directory_path() / "fo_test_file.txt";
        std::ofstream ofs(test_file);
        ofs << "Hello, World!";
        ofs.close();
    }
    
    void TearDown() override {
        if (std::filesystem::exists(test_file)) {
            std::filesystem::remove(test_file);
        }
    }
    
    std::filesystem::path test_file;
};

TEST_F(HasherTest, Fast64HasherExists) {
    auto hasher = Registry<IHasher>::instance().create("fast64");
    ASSERT_NE(hasher, nullptr);
    EXPECT_EQ(hasher->name(), "fast64");
}

TEST_F(HasherTest, Fast64ProducesConsistentHash) {
    auto hasher = Registry<IHasher>::instance().create("fast64");
    ASSERT_NE(hasher, nullptr);
    
    std::string hash1 = hasher->fast64(test_file);
    std::string hash2 = hasher->fast64(test_file);
    
    EXPECT_FALSE(hash1.empty());
    EXPECT_EQ(hash1, hash2);
}

TEST_F(HasherTest, Fast64DifferentFilesProduceDifferentHashes) {
    auto hasher = Registry<IHasher>::instance().create("fast64");
    ASSERT_NE(hasher, nullptr);
    
    // Create another test file with different content
    auto test_file2 = std::filesystem::temp_directory_path() / "fo_test_file2.txt";
    std::ofstream ofs(test_file2);
    ofs << "Different content!";
    ofs.close();
    
    std::string hash1 = hasher->fast64(test_file);
    std::string hash2 = hasher->fast64(test_file2);
    
    EXPECT_NE(hash1, hash2);
    
    std::filesystem::remove(test_file2);
}

TEST_F(HasherTest, Sha256HasherExists) {
    auto hasher = Registry<IHasher>::instance().create("sha256");
    ASSERT_NE(hasher, nullptr);
    EXPECT_EQ(hasher->name(), "sha256");
}

TEST_F(HasherTest, Sha256ProducesValidHash) {
    auto hasher = Registry<IHasher>::instance().create("sha256");
    ASSERT_NE(hasher, nullptr);
    
    std::string hash = hasher->full(test_file);
    
    // SHA256 produces 64 hex characters
    EXPECT_EQ(hash.length(), 64);
    
    // Should only contain hex characters
    for (char c : hash) {
        EXPECT_TRUE((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
    }
}

TEST_F(HasherTest, XxHashHasherExists) {
    auto hasher = Registry<IHasher>::instance().create("xxhash");
    ASSERT_NE(hasher, nullptr);
    EXPECT_EQ(hasher->name(), "xxhash");
}

TEST_F(HasherTest, ListAvailableHashers) {
    auto names = Registry<IHasher>::instance().list();
    
    EXPECT_FALSE(names.empty());
    
    // Should have at least fast64
    bool has_fast64 = false;
    for (const auto& name : names) {
        if (name == "fast64") has_fast64 = true;
    }
    EXPECT_TRUE(has_fast64);
}

