// Advanced Key ID System Test
// Demonstrates efficient layout conversion with UTF-8 support

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

// Key ID System with UTF-8 support
class AdvancedKeyIDSystem {
public:
    // Key ID format: FamilyID * 1000 + LayoutID * 100 + KeyPosition
    static int generate_key_id(int family_id, int layout_id, int key_position) {
        return family_id * 1000 + layout_id * 100 + key_position;
    }
    
    // Layout definition supporting UTF-8
    struct Layout {
        std::string id;
        std::string name;
        int family_id;
        int layout_id;
        std::unordered_map<int, std::string> key_to_char;  // KeyID -> UTF-8 character
        std::unordered_map<std::string, int> char_to_key;  // UTF-8 character -> KeyID
        double frequency_score;
    };
    
    // Demo layouts
    static void create_demo_layouts() {
        // QWERTY layout
        Layout qwerty;
        qwerty.id = "qwerty";
        qwerty.name = "QWERTY";
        qwerty.family_id = 1;  // Latin
        qwerty.layout_id = 1;
        qwerty.frequency_score = 0.9;
        
        std::vector<std::string> qwerty_chars = {
            "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
            "a", "s", "d", "f", "g", "h", "j", "k", "l",
            "z", "x", "c", "v", "b", "n", "m"
        };
        
        for (int i = 0; i < 26; ++i) {
            int key_id = generate_key_id(qwerty.family_id, qwerty.layout_id, i + 1);
            qwerty.key_to_char[key_id] = qwerty_chars[i];
            qwerty.char_to_key[qwerty_chars[i]] = key_id;
        }
        
        // Workman layout
        Layout workman;
        workman.id = "workman";
        workman.name = "Workman";
        workman.family_id = 1;  // Latin
        workman.layout_id = 2;
        workman.frequency_score = 0.05;
        
        std::vector<std::string> workman_chars = {
            "d", "r", "w", "b", "j", "f", "u", "p", ";", "l",
            "a", "s", "h", "t", "g", "y", "n", "e", "o",
            "z", "x", "m", "c", "v", "k", "l"
        };
        
        for (int i = 0; i < 26; ++i) {
            int key_id = generate_key_id(workman.family_id, workman.layout_id, i + 1);
            workman.key_to_char[key_id] = workman_chars[i];
            workman.char_to_key[workman_chars[i]] = key_id;
        }
        
        // Russian layout
        Layout russian;
        russian.id = "russian";
        russian.name = "Russian";
        russian.family_id = 2;  // Cyrillic
        russian.layout_id = 1;
        russian.frequency_score = 0.8;
        
        std::vector<std::string> russian_chars = {
            "й", "ц", "у", "к", "е", "н", "г", "ш", "щ", "з",
            "ф", "ы", "в", "а", "п", "р", "о", "л", "д",
            "я", "ч", "с", "м", "и", "т", "ь"
        };
        
        for (int i = 0; i < 26; ++i) {
            int key_id = generate_key_id(russian.family_id, russian.layout_id, i + 1);
            russian.key_to_char[key_id] = russian_chars[i];
            russian.char_to_key[russian_chars[i]] = key_id;
        }
        
        layouts_["qwerty"] = qwerty;
        layouts_["workman"] = workman;
        layouts_["russian"] = russian;
    }
    
    // Convert text using key IDs
    static std::string convert_text(const std::string& text, const std::string& from_layout, const std::string& to_layout) {
        auto from_it = layouts_.find(from_layout);
        auto to_it = layouts_.find(to_layout);
        
        if (from_it == layouts_.end() || to_it == layouts_.end()) {
            return text;  // Return original if layouts not found
        }
        
        const Layout& from = from_it->second;
        const Layout& to = to_it->second;
        
        std::string result;
        result.reserve(text.length() * 2);  // Reserve space for UTF-8
        
        size_t i = 0;
        while (i < text.length()) {
            // Extract UTF-8 character
            std::string utf8_char = extract_utf8_char(text, i);
            
            // Get key ID for character in source layout
            auto char_it = from.char_to_key.find(utf8_char);
            if (char_it == from.char_to_key.end()) {
                result += utf8_char;  // Character not found, keep original
                continue;
            }
            
            int key_id = char_it->second;
            
            // Extract key position from key ID
            int key_position = key_id % 100;
            
            // Generate key ID for target layout
            int target_key_id = generate_key_id(to.family_id, to.layout_id, key_position);
            
            // Get character for target key ID
            auto key_it = to.key_to_char.find(target_key_id);
            if (key_it == to.key_to_char.end()) {
                result += utf8_char;  // No mapping found, keep original
                continue;
            }
            
            result += key_it->second;
        }
        
        return result;
    }
    
    // Detect likely layouts
    static std::vector<std::string> detect_layouts(const std::string& text) {
        std::vector<std::pair<std::string, double>> scores;
        
        for (const auto& [layout_id, layout] : layouts_) {
            double score = calculate_score(text, layout);
            if (score > 0.1) {
                scores.emplace_back(layout_id, score);
            }
        }
        
        // Sort by score
        std::sort(scores.begin(), scores.end(), 
                 [](const auto& a, const auto& b) { return a.second > b.second; });
        
        std::vector<std::string> result;
        for (const auto& [layout_id, score] : scores) {
            result.push_back(layout_id);
        }
        
        return result;
    }
    
    // Get available layouts
    static std::vector<std::string> get_available_layouts() {
        std::vector<std::string> result;
        for (const auto& [id, layout] : layouts_) {
            result.push_back(id);
        }
        return result;
    }

private:
    static std::unordered_map<std::string, Layout> layouts_;
    
    static std::string extract_utf8_char(const std::string& text, size_t& pos) {
        if (pos >= text.length()) return "";
        
        unsigned char first_byte = static_cast<unsigned char>(text[pos]);
        
        if (first_byte < 0x80) {
            // ASCII character
            return std::string(1, text[pos++]);
        } else if (first_byte < 0xC0) {
            // Invalid UTF-8
            pos++;
            return "?";
        } else if (first_byte < 0xE0) {
            // 2-byte UTF-8
            if (pos + 1 < text.length()) {
                std::string result = text.substr(pos, 2);
                pos += 2;
                return result;
            }
        } else if (first_byte < 0xF0) {
            // 3-byte UTF-8
            if (pos + 2 < text.length()) {
                std::string result = text.substr(pos, 3);
                pos += 3;
                return result;
            }
        }
        
        // Fallback
        pos++;
        return "?";
    }
    
    static double calculate_score(const std::string& text, const Layout& layout) {
        double score = 0.0;
        
        // Character frequency analysis
        std::unordered_map<std::string, int> char_count;
        size_t i = 0;
        while (i < text.length()) {
            std::string utf8_char = extract_utf8_char(text, i);
            char_count[utf8_char]++;
        }
        
        int found_chars = 0;
        int total_chars = 0;
        
        for (const auto& [c, count] : char_count) {
            total_chars += count;
            if (layout.char_to_key.find(c) != layout.char_to_key.end()) {
                found_chars += count;
            }
        }
        
        if (total_chars > 0) {
            score += static_cast<double>(found_chars) / total_chars;
        }
        
        // Base frequency score
        score += layout.frequency_score * 0.1;
        
        return score;
    }
};

std::unordered_map<std::string, AdvancedKeyIDSystem::Layout> AdvancedKeyIDSystem::layouts_;

int main() {
    std::cout << "🚀 Advanced Key ID System Demo\n";
    std::cout << "==============================\n\n";
    
    // Initialize layouts
    AdvancedKeyIDSystem::create_demo_layouts();
    
    // Test conversions
    std::cout << "🔄 Layout Conversions:\n\n";
    
    std::vector<std::pair<std::string, std::string>> test_cases = {
        {"hello", "qwerty"},
        {"ywoo;", "workman"},
        {"привет", "russian"}
    };
    
    for (const auto& [text, expected_layout] : test_cases) {
        std::cout << "Text: '" << text << "'\n";
        
        // Detect layout
        auto detected = AdvancedKeyIDSystem::detect_layouts(text);
        std::cout << "Detected layouts: ";
        for (size_t i = 0; i < detected.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << detected[i];
        }
        std::cout << "\n";
        
        // Test conversions
        if (detected.size() >= 2) {
            std::string converted = AdvancedKeyIDSystem::convert_text(text, detected[0], detected[1]);
            std::cout << "Converted (" << detected[0] << " -> " << detected[1] << "): '" << converted << "'\n";
        }
        std::cout << "\n";
    }
    
    // Performance demo
    std::cout << "⚡ Performance Benefits:\n\n";
    std::cout << "Key ID System Advantages:\n";
    std::cout << "✅ Direct key position mapping (O(1))\n";
    std::cout << "✅ No intermediate conversions needed\n";
    std::cout << "✅ Memory efficient - only load needed layouts\n";
    std::cout << "✅ UTF-8 support for international layouts\n";
    std::cout << "✅ Easy to add new layouts and families\n\n";
    
    std::cout << "Available layouts: ";
    auto layouts = AdvancedKeyIDSystem::get_available_layouts();
    for (size_t i = 0; i < layouts.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << layouts[i];
    }
    std::cout << "\n\n";
    
    std::cout << "✅ Advanced demo completed!\n";
    std::cout << "\nReady for:\n";
    std::cout << "1. macOS add-on integration\n";
    std::cout << "2. Telegram bot enhancement\n";
    std::cout << "3. Web UI development\n";
    
    return 0;
} 