// Key ID System Implementation
// Efficient layout conversion using key IDs

#include "../include/key_system.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <string>
#include <memory>

using json = nlohmann::json;

namespace layout_converter {

// KeyUtils implementation
namespace KeyUtils {
    int char_to_key_position(char c) {
        char lower = std::tolower(c);
        if (lower >= 'a' && lower <= 'z') {
            return lower - 'a' + 1;  // A=1, B=2, etc.
        }
        return 0;  // Invalid
    }
    
    char key_position_to_char(int position) {
        if (position >= 1 && position <= 26) {
            return 'a' + position - 1;  // 1=A, 2=B, etc.
        }
        return '\0';  // Invalid
    }
    
    int get_key_id_for_char(char c, const LayoutDefinition& layout) {
        auto it = layout.char_to_key.find(c);
        return it != layout.char_to_key.end() ? it->second : 0;
    }
    
    char get_char_for_key_id(int key_id, const LayoutDefinition& layout) {
        auto it = layout.key_to_char.find(key_id);
        return it != layout.key_to_char.end() ? it->second : c;
    }
}

// KeyBasedLayoutLibrary implementation
class KeyBasedLayoutLibrary::Impl {
public:
    Impl() = default;
    ~Impl() = default;
    
    bool load_layout(const std::string& layout_id, const std::string& file_path) {
        try {
            std::ifstream file(file_path);
            if (!file.is_open()) {
                return false;
            }
            
            json j;
            file >> j;
            
            auto layout = std::make_shared<LayoutDefinition>();
            layout->id = j["id"];
            layout->name = j["name"];
            layout->family_id = j["family_id"];
            layout->layout_id = j["layout_id"];
            layout->frequency_score = j["frequency_score"];
            
            // Load common words
            if (j.contains("common_words")) {
                layout->common_words = j["common_words"].get<std::vector<std::string>>();
            }
            
            // Load key mappings
            auto key_mappings = j["key_mappings"];
            for (auto it = key_mappings.begin(); it != key_mappings.end(); ++it) {
                int key_id = std::stoi(it.key());
                char character = it.value().get<std::string>()[0];
                
                layout->key_to_char[key_id] = character;
                layout->char_to_key[character] = key_id;
            }
            
            layouts_[layout_id] = layout;
            return true;
            
        } catch (const std::exception& e) {
            return false;
        }
    }
    
    std::shared_ptr<LayoutDefinition> get_layout(const std::string& layout_id) {
        auto it = layouts_.find(layout_id);
        if (it != layouts_.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    std::string convert_text(const std::string& text, 
                           const std::string& from_layout_id, 
                           const std::string& to_layout_id) {
        auto from_layout = get_layout(from_layout_id);
        auto to_layout = get_layout(to_layout_id);
        
        if (!from_layout || !to_layout) {
            return text;  // Return original if layouts not found
        }
        
        std::string result;
        result.reserve(text.length());
        
        for (char c : text) {
            char converted = convert_char(c, *from_layout, *to_layout);
            result += converted;
        }
        
        return result;
    }
    
    std::vector<std::string> detect_likely_layouts(const std::string& text, 
                                                  const std::string& user_language) {
        std::vector<std::pair<std::string, double>> scores;
        
        for (const auto& [layout_id, layout] : layouts_) {
            double score = calculate_layout_score(text, *layout, user_language);
            if (score > 0.1) {  // Threshold
                scores.emplace_back(layout_id, score);
            }
        }
        
        // Sort by score (highest first)
        std::sort(scores.begin(), scores.end(), 
                 [](const auto& a, const auto& b) { return a.second > b.second; });
        
        std::vector<std::string> result;
        for (const auto& [layout_id, score] : scores) {
            result.push_back(layout_id);
        }
        
        return result;
    }
    
    std::vector<std::string> get_loaded_layouts() const {
        std::vector<std::string> result;
        for (const auto& [layout_id, layout] : layouts_) {
            result.push_back(layout_id);
        }
        return result;
    }
    
    void clear_cache() {
        layouts_.clear();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<LayoutDefinition>> layouts_;
    
    char convert_char(char c, const LayoutDefinition& from_layout, const LayoutDefinition& to_layout) {
        // Get key ID for character in source layout
        int key_id = KeyUtils::get_key_id_for_char(c, from_layout);
        if (key_id == 0) {
            return c;  // Character not found, return original
        }
        
        // Extract key position from key ID
        KeyIDComponents components(key_id);
        
        // Generate key ID for target layout
        int target_key_id = generate_key_id(to_layout.family_id, to_layout.layout_id, components.key_position);
        
        // Get character for target key ID
        char result = KeyUtils::get_char_for_key_id(target_key_id, to_layout);
        if (result == '\0') {
            return c;  // No mapping found, return original
        }
        
        // Preserve case
        return std::isupper(c) ? std::toupper(result) : result;
    }
    
    double calculate_layout_score(const std::string& text, const LayoutDefinition& layout, 
                                const std::string& user_language) {
        double score = 0.0;
        
        // Character frequency analysis
        score += analyze_character_frequency(text, layout);
        
        // Common word analysis
        score += analyze_common_words(text, layout);
        
        // Language compatibility
        score += analyze_language_compatibility(text, layout, user_language);
        
        // Base frequency score
        score += layout.frequency_score * 0.1;
        
        return score;
    }
    
    double analyze_character_frequency(const std::string& text, const LayoutDefinition& layout) {
        if (text.empty()) return 0.0;
        
        std::unordered_map<char, int> char_count;
        for (char c : text) {
            if (std::isalpha(c)) {
                char_count[std::tolower(c)]++;
            }
        }
        
        // Check if characters exist in layout
        int found_chars = 0;
        int total_chars = 0;
        
        for (const auto& [c, count] : char_count) {
            total_chars += count;
            if (layout.char_to_key.find(c) != layout.char_to_key.end()) {
                found_chars += count;
            }
        }
        
        return total_chars > 0 ? static_cast<double>(found_chars) / total_chars : 0.0;
    }
    
    double analyze_common_words(const std::string& text, const LayoutDefinition& layout) {
        if (layout.common_words.empty()) return 0.0;
        
        std::string lower_text = text;
        std::transform(lower_text.begin(), lower_text.end(), lower_text.begin(), ::tolower);
        
        int found_words = 0;
        for (const std::string& word : layout.common_words) {
            if (lower_text.find(word) != std::string::npos) {
                found_words++;
            }
        }
        
        return static_cast<double>(found_words) / layout.common_words.size();
    }
    
    double analyze_language_compatibility(const std::string& text, const LayoutDefinition& layout, 
                                        const std::string& user_language) {
        // Simple script detection
        bool has_cyrillic = false;
        bool has_latin = false;
        
        for (char c : text) {
            if (c >= 0x0400 && c <= 0x04FF) {  // Cyrillic range
                has_cyrillic = true;
            } else if (std::isalpha(c)) {
                has_latin = true;
            }
        }
        
        // Score based on script compatibility
        if (layout.family_id == KeyID::FAMILY_CYRILLIC && has_cyrillic) {
            return 0.5;
        } else if (layout.family_id == KeyID::FAMILY_LATIN && has_latin) {
            return 0.5;
        }
        
        return 0.0;
    }
};

// KeyBasedLayoutLibrary public methods
KeyBasedLayoutLibrary::KeyBasedLayoutLibrary() : pImpl(std::make_unique<Impl>()) {}
KeyBasedLayoutLibrary::~KeyBasedLayoutLibrary() = default;

bool KeyBasedLayoutLibrary::load_layout(const std::string& layout_id, const std::string& file_path) {
    return pImpl->load_layout(layout_id, file_path);
}

std::shared_ptr<LayoutDefinition> KeyBasedLayoutLibrary::get_layout(const std::string& layout_id) {
    return pImpl->get_layout(layout_id);
}

std::string KeyBasedLayoutLibrary::convert_text(const std::string& text, 
                                              const std::string& from_layout_id, 
                                              const std::string& to_layout_id) {
    return pImpl->convert_text(text, from_layout_id, to_layout_id);
}

std::vector<std::string> KeyBasedLayoutLibrary::detect_likely_layouts(const std::string& text, 
                                                                     const std::string& user_language) {
    return pImpl->detect_likely_layouts(text, user_language);
}

std::vector<std::string> KeyBasedLayoutLibrary::get_loaded_layouts() const {
    return pImpl->get_loaded_layouts();
}

void KeyBasedLayoutLibrary::clear_cache() {
    pImpl->clear_cache();
}

} // namespace layout_converter 