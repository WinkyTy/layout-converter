// Dynamic Layout Library System
// Loads layouts on-demand based on intelligent detection

#ifndef LAYOUT_LIBRARY_H
#define LAYOUT_LIBRARY_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <optional>

namespace layout_converter {

// Layout definition structure
struct LayoutDefinition {
    std::string id;
    std::string name;
    std::string language;  // "en", "ru", "hi", etc.
    std::string script;    // "latin", "cyrillic", "devanagari", etc.
    std::unordered_map<char, char> mapping;
    double frequency_score;  // How common this layout is
    std::vector<std::string> common_words;  // Words that indicate this layout
};

// Layout detection result
struct DetectionHint {
    std::string layout_id;
    double confidence;
    std::string reason;  // "character_frequency", "word_pattern", "user_preference"
};

// Dynamic layout library
class LayoutLibrary {
public:
    LayoutLibrary();
    ~LayoutLibrary();
    
    // Load layout from file
    bool load_layout(const std::string& layout_id, const std::string& file_path);
    
    // Get layout by ID (loads if not already loaded)
    std::shared_ptr<LayoutDefinition> get_layout(const std::string& layout_id);
    
    // Smart detection - returns most likely layouts
    std::vector<DetectionHint> detect_likely_layouts(const std::string& text, 
                                                    const std::string& user_language = "en");
    
    // Convert text between layouts (loads layouts if needed)
    std::string convert_text(const std::string& text, 
                           const std::string& from_layout_id, 
                           const std::string& to_layout_id);
    
    // Get all loaded layouts
    std::vector<std::string> get_loaded_layouts() const;
    
    // Preload common layouts for a language
    void preload_language_layouts(const std::string& language);
    
    // Clear loaded layouts (free memory)
    void clear_cache();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

// Layout detection algorithms
class LayoutDetector {
public:
    // Analyze text and return layout hints
    static std::vector<DetectionHint> analyze_text(const std::string& text, 
                                                  const std::string& user_language);
    
    // Check character frequency patterns
    static double check_character_frequency(const std::string& text, 
                                          const std::string& layout_id);
    
    // Check for common words in layout
    static double check_common_words(const std::string& text, 
                                   const std::vector<std::string>& common_words);
    
    // Check script/language compatibility
    static double check_script_compatibility(const std::string& text, 
                                           const std::string& script);
};

} // namespace layout_converter

#endif // LAYOUT_LIBRARY_H 