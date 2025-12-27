#include <gtest/gtest.h>
#include "fo/core/rule_engine.hpp"
#include "fo/core/types.hpp"
#include <chrono>

using namespace fo::core;

TEST(RuleEngineTest, SimpleExpansion) {
    RuleEngine engine;
    engine.add_rule({"Simple", "", "output/{name}.{ext}"});

    FileInfo file;
    file.path = "source/test_file.txt";
    
    auto result = engine.apply_rules(file, {});
    ASSERT_TRUE(result.has_value());
    // Use generic_string to avoid path separator issues in comparison
    EXPECT_EQ(result->generic_string(), "output/test_file.txt");
}

TEST(RuleEngineTest, TagFilter) {
    RuleEngine engine;
    engine.add_rule({"TagRule", "document", "docs/{name}.{ext}"});

    FileInfo file;
    file.path = "source/invoice.pdf";

    // Should not match without tag
    auto result1 = engine.apply_rules(file, {});
    EXPECT_FALSE(result1.has_value());

    // Should match with tag
    auto result2 = engine.apply_rules(file, {"document"});
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->generic_string(), "docs/invoice.pdf");
}

TEST(RuleEngineTest, ParentVariable) {
    RuleEngine engine;
    engine.add_rule({"ParentRule", "", "{parent}_renamed/{name}.{ext}"});

    FileInfo file;
    file.path = "folder/subfolder/image.png";

    auto result = engine.apply_rules(file, {});
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->generic_string(), "subfolder_renamed/image.png");
}
