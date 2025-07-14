// Test file for Layout Converter
// Basic unit tests for the layout converter functionality

#include "../core/include/layout_converter.h"
#include <gtest/gtest.h>
#include <string>

class LayoutConverterTest : public ::testing::Test {
protected:
    void SetUp() override {
        converter = std::make_unique<layout_converter::LayoutConverter>();
    }

    std::unique_ptr<layout_converter::LayoutConverter> converter;
};

TEST_F(LayoutConverterTest, BasicConversion) {
    // Test basic QWERTY to Workman conversion
    auto result = converter->convert("hello", 
                                   layout_converter::LayoutType::QWERTY, 
                                   layout_converter::LayoutType::WORKMAN);
    
    EXPECT_EQ(result.original_text, "hello");
    EXPECT_EQ(result.from_layout, layout_converter::LayoutType::QWERTY);
    EXPECT_EQ(result.to_layout, layout_converter::LayoutType::WORKMAN);
    EXPECT_EQ(result.confidence, 1.0);
}

TEST_F(LayoutConverterTest, SameLayoutConversion) {
    // Test conversion to same layout
    auto result = converter->convert("hello", 
                                   layout_converter::LayoutType::QWERTY, 
                                   layout_converter::LayoutType::QWERTY);
    
    EXPECT_EQ(result.original_text, "hello");
    EXPECT_EQ(result.converted_text, "hello");
}

TEST_F(LayoutConverterTest, LayoutTypeToString) {
    EXPECT_EQ(layout_converter::layout_type_to_string(layout_converter::LayoutType::QWERTY), "QWERTY");
    EXPECT_EQ(layout_converter::layout_type_to_string(layout_converter::LayoutType::WORKMAN), "Workman");
    EXPECT_EQ(layout_converter::layout_type_to_string(layout_converter::LayoutType::COLEMAK), "Colemak");
}

TEST_F(LayoutConverterTest, StringToLayoutType) {
    EXPECT_EQ(layout_converter::string_to_layout_type("qwerty"), layout_converter::LayoutType::QWERTY);
    EXPECT_EQ(layout_converter::string_to_layout_type("workman"), layout_converter::LayoutType::WORKMAN);
    EXPECT_EQ(layout_converter::string_to_layout_type("unknown"), layout_converter::LayoutType::UNKNOWN);
}

TEST_F(LayoutConverterTest, DetectAndConvert) {
    // Test auto-detection
    auto result = converter->detect_and_convert("hello");
    
    EXPECT_EQ(result.text, "hello");
    EXPECT_FALSE(result.possible_conversions.empty());
    
    // Should have at least one conversion
    EXPECT_GT(result.possible_conversions.size(), 0);
}

TEST_F(LayoutConverterTest, IsValidLayoutType) {
    EXPECT_TRUE(layout_converter::is_valid_layout_type(layout_converter::LayoutType::QWERTY));
    EXPECT_TRUE(layout_converter::is_valid_layout_type(layout_converter::LayoutType::WORKMAN));
    EXPECT_FALSE(layout_converter::is_valid_layout_type(layout_converter::LayoutType::UNKNOWN));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 