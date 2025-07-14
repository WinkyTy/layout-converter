// Layout Converter Core Library
// High-performance keyboard layout conversion and detection

#ifndef LAYOUT_CONVERTER_H
#define LAYOUT_CONVERTER_H

#ifdef _WIN32
    #ifdef LAYOUT_CONVERTER_EXPORTS
        #define LAYOUT_CONVERTER_API __declspec(dllexport)
    #else
        #define LAYOUT_CONVERTER_API __declspec(dllimport)
    #endif
#else
    #define LAYOUT_CONVERTER_API __attribute__((visibility("default")))
#endif

#include <string>
#include <vector>
#include <memory>

namespace layout_converter {

// Forward declarations
class LayoutEngine;
class LayoutDetector;
class ProbabilityScorer;

// Layout types enumeration
enum class LayoutType {
    QWERTY,
    CYRILLIC,
    COLEMAK,
    WORKMAN,
    DVORAK,
    RUSSIAN_TYPEWRITER,
    UNKNOWN
};

// Conversion result structure
struct ConversionResult {
    std::string original_text;
    std::string converted_text;
    LayoutType from_layout;
    LayoutType to_layout;
    double confidence;
    
    ConversionResult() : confidence(0.0) {}
};

// Detection result structure
struct DetectionResult {
    std::string text;
    std::vector<ConversionResult> possible_conversions;
    
    DetectionResult() = default;
};

// Main converter class
class LAYOUT_CONVERTER_API LayoutConverter {
public:
    LayoutConverter();
    ~LayoutConverter();
    
    // Disable copy constructor and assignment
    LayoutConverter(const LayoutConverter&) = delete;
    LayoutConverter& operator=(const LayoutConverter&) = delete;
    
    // Enable move constructor and assignment
    LayoutConverter(LayoutConverter&&) noexcept;
    LayoutConverter& operator=(LayoutConverter&&) noexcept;
    
    // Core conversion methods
    ConversionResult convert(const std::string& text, 
                           LayoutType from_layout, 
                           LayoutType to_layout);
    
    // Smart detection methods
    DetectionResult detect_and_convert(const std::string& text);
    
    // Batch processing
    std::vector<ConversionResult> batch_convert(
        const std::vector<std::string>& texts,
        LayoutType from_layout,
        LayoutType to_layout);
    
    // Layout management
    bool add_custom_layout(const std::string& name, 
                          const std::string& layout_data);
    bool remove_custom_layout(const std::string& name);
    std::vector<std::string> get_available_layouts() const;
    
    // Configuration
    void set_detection_threshold(double threshold);
    void set_max_results(size_t max_results);
    double get_detection_threshold() const;
    size_t get_max_results() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

// Utility functions
LAYOUT_CONVERTER_API std::string layout_type_to_string(LayoutType type);
LAYOUT_CONVERTER_API LayoutType string_to_layout_type(const std::string& str);
LAYOUT_CONVERTER_API bool is_valid_layout_type(LayoutType type);

// Error handling
class LAYOUT_CONVERTER_API LayoutConverterException : public std::exception {
public:
    explicit LayoutConverterException(const std::string& message);
    const char* what() const noexcept override;
    
private:
    std::string message_;
};

} // namespace layout_converter

#endif // LAYOUT_CONVERTER_H 