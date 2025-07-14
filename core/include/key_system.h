// Key ID System
// Assigns unique IDs to keyboard keys for efficient layout conversion

#ifndef KEY_SYSTEM_H
#define KEY_SYSTEM_H

#include <string>
#include <unordered_map>
#include <vector>

namespace layout_converter {

// Key ID constants
namespace KeyID {
    // Family IDs (first digit)
    constexpr int FAMILY_LATIN = 1;
    constexpr int FAMILY_CYRILLIC = 2;
    constexpr int FAMILY_HINDI = 3;
    constexpr int FAMILY_ARABIC = 4;
    constexpr int FAMILY_CHINESE = 5;
    
    // Layout IDs (second digit)
    constexpr int LAYOUT_QWERTY = 1;
    constexpr int LAYOUT_WORKMAN = 2;
    constexpr int LAYOUT_COLEMAK = 3;
    constexpr int LAYOUT_DVORAK = 4;
    constexpr int LAYOUT_RUSSIAN = 1;  // Russian is layout 1 in Cyrillic family
    
    // Key positions (A-Z = 1-26)
    constexpr int KEY_A = 1;
    constexpr int KEY_B = 2;
    constexpr int KEY_C = 3;
    constexpr int KEY_D = 4;
    constexpr int KEY_E = 5;
    constexpr int KEY_F = 6;
    constexpr int KEY_G = 7;
    constexpr int KEY_H = 8;
    constexpr int KEY_I = 9;
    constexpr int KEY_J = 10;
    constexpr int KEY_K = 11;
    constexpr int KEY_L = 12;
    constexpr int KEY_M = 13;
    constexpr int KEY_N = 14;
    constexpr int KEY_O = 15;
    constexpr int KEY_P = 16;
    constexpr int KEY_Q = 17;
    constexpr int KEY_R = 18;
    constexpr int KEY_S = 19;
    constexpr int KEY_T = 20;
    constexpr int KEY_U = 21;
    constexpr int KEY_V = 22;
    constexpr int KEY_W = 23;
    constexpr int KEY_X = 24;
    constexpr int KEY_Y = 25;
    constexpr int KEY_Z = 26;
}

// Generate key ID: FamilyID * 1000 + LayoutID * 100 + KeyPosition
inline int generate_key_id(int family_id, int layout_id, int key_position) {
    return family_id * 1000 + layout_id * 100 + key_position;
}

// Parse key ID into components
struct KeyIDComponents {
    int family_id;
    int layout_id;
    int key_position;
    
    KeyIDComponents(int key_id) {
        family_id = key_id / 1000;
        layout_id = (key_id % 1000) / 100;
        key_position = key_id % 100;
    }
};

// Layout definition using key IDs
struct LayoutDefinition {
    std::string id;
    std::string name;
    int family_id;
    int layout_id;
    std::unordered_map<int, char> key_to_char;  // KeyID -> Character
    std::unordered_map<char, int> char_to_key;  // Character -> KeyID
    double frequency_score;
    std::vector<std::string> common_words;
};

// Layout library using key IDs
class KeyBasedLayoutLibrary {
public:
    KeyBasedLayoutLibrary();
    ~KeyBasedLayoutLibrary();
    
    // Load layout from JSON file
    bool load_layout(const std::string& layout_id, const std::string& file_path);
    
    // Get layout by ID
    std::shared_ptr<LayoutDefinition> get_layout(const std::string& layout_id);
    
    // Convert text using key IDs (most efficient)
    std::string convert_text(const std::string& text, 
                           const std::string& from_layout_id, 
                           const std::string& to_layout_id);
    
    // Smart detection using key patterns
    std::vector<std::string> detect_likely_layouts(const std::string& text, 
                                                  const std::string& user_language = "en");
    
    // Get all loaded layouts
    std::vector<std::string> get_loaded_layouts() const;
    
    // Clear cache
    void clear_cache();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

// Utility functions
namespace KeyUtils {
    // Convert character to key position (A=1, B=2, etc.)
    int char_to_key_position(char c);
    
    // Convert key position to character (1=A, 2=B, etc.)
    char key_position_to_char(int position);
    
    // Get key ID for a character in a specific layout
    int get_key_id_for_char(char c, const LayoutDefinition& layout);
    
    // Get character for a key ID in a specific layout
    char get_char_for_key_id(int key_id, const LayoutDefinition& layout);
}

} // namespace layout_converter

#endif // KEY_SYSTEM_H 