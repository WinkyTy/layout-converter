// Simple Layout Loader
// Loads layouts using key IDs without external dependencies

#include "../include/key_system.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace layout_converter {

class SimpleLayoutLoader {
public:
    static bool load_layout_from_file(const std::string& file_path, LayoutDefinition& layout) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Failed to open layout file: " << file_path << std::endl;
            return false;
        }
        
        std::string line;
        bool in_key_mappings = false;
        
        while (std::getline(file, line)) {
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            // Parse basic info
            if (line.find("\"id\":") != std::string::npos) {
                layout.id = extract_string_value(line);
            } else if (line.find("\"name\":") != std::string::npos) {
                layout.name = extract_string_value(line);
            } else if (line.find("\"family_id\":") != std::string::npos) {
                layout.family_id = extract_int_value(line);
            } else if (line.find("\"layout_id\":") != std::string::npos) {
                layout.layout_id = extract_int_value(line);
            } else if (line.find("\"frequency_score\":") != std::string::npos) {
                layout.frequency_score = extract_double_value(line);
            } else if (line.find("\"key_mappings\":") != std::string::npos) {
                in_key_mappings = true;
            } else if (in_key_mappings && line.find("}") != std::string::npos) {
                in_key_mappings = false;
            } else if (in_key_mappings && line.find(":") != std::string::npos) {
                parse_key_mapping(line, layout);
            }
        }
        
        return true;
    }
    
    static void create_demo_layouts() {
        // Create QWERTY layout
        LayoutDefinition qwerty;
        qwerty.id = "qwerty";
        qwerty.name = "QWERTY";
        qwerty.family_id = KeyID::FAMILY_LATIN;
        qwerty.layout_id = KeyID::LAYOUT_QWERTY;
        qwerty.frequency_score = 0.9;
        
        // QWERTY key mappings (key position -> character)
        std::vector<char> qwerty_chars = {
            'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
            'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
            'z', 'x', 'c', 'v', 'b', 'n', 'm'
        };
        
        for (int i = 0; i < 26; ++i) {
            int key_id = generate_key_id(qwerty.family_id, qwerty.layout_id, i + 1);
            qwerty.key_to_char[key_id] = qwerty_chars[i];
            qwerty.char_to_key[qwerty_chars[i]] = key_id;
        }
        
        // Create Workman layout
        LayoutDefinition workman;
        workman.id = "workman";
        workman.name = "Workman";
        workman.family_id = KeyID::FAMILY_LATIN;
        workman.layout_id = KeyID::LAYOUT_WORKMAN;
        workman.frequency_score = 0.05;
        
        // Workman key mappings
        std::vector<char> workman_chars = {
            'd', 'r', 'w', 'b', 'j', 'f', 'u', 'p', ';', 'l',
            'a', 's', 'h', 't', 'g', 'y', 'n', 'e', 'o',
            'z', 'x', 'm', 'c', 'v', 'k', 'l'
        };
        
        for (int i = 0; i < 26; ++i) {
            int key_id = generate_key_id(workman.family_id, workman.layout_id, i + 1);
            workman.key_to_char[key_id] = workman_chars[i];
            workman.char_to_key[workman_chars[i]] = key_id;
        }
        
        // Create Russian layout
        LayoutDefinition russian;
        russian.id = "russian";
        russian.name = "Russian";
        russian.family_id = KeyID::FAMILY_CYRILLIC;
        russian.layout_id = KeyID::LAYOUT_RUSSIAN;
        russian.frequency_score = 0.8;
        
        // Russian key mappings (using UTF-8 strings)
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
        
        // Store layouts
        demo_layouts_["qwerty"] = qwerty;
        demo_layouts_["workman"] = workman;
        demo_layouts_["russian"] = russian;
    }
    
    static std::shared_ptr<LayoutDefinition> get_demo_layout(const std::string& layout_id) {
        auto it = demo_layouts_.find(layout_id);
        if (it != demo_layouts_.end()) {
            return std::make_shared<LayoutDefinition>(it->second);
        }
        return nullptr;
    }
    
    static std::vector<std::string> get_available_layouts() {
        std::vector<std::string> layouts;
        for (const auto& [id, layout] : demo_layouts_) {
            layouts.push_back(id);
        }
        return layouts;
    }

private:
    static std::unordered_map<std::string, LayoutDefinition> demo_layouts_;
    
    static std::string extract_string_value(const std::string& line) {
        size_t start = line.find('"', line.find('"') + 1) + 1;
        size_t end = line.find('"', start);
        return line.substr(start, end - start);
    }
    
    static int extract_int_value(const std::string& line) {
        size_t start = line.find(':') + 1;
        while (start < line.length() && (line[start] == ' ' || line[start] == '\t')) ++start;
        return std::stoi(line.substr(start));
    }
    
    static double extract_double_value(const std::string& line) {
        size_t start = line.find(':') + 1;
        while (start < line.length() && (line[start] == ' ' || line[start] == '\t')) ++start;
        return std::stod(line.substr(start));
    }
    
    static void parse_key_mapping(const std::string& line, LayoutDefinition& layout) {
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) return;
        
        std::string key_id_str = line.substr(0, colon_pos);
        std::string char_str = line.substr(colon_pos + 1);
        
        // Clean up strings
        key_id_str.erase(std::remove(key_id_str.begin(), key_id_str.end(), '"'), key_id_str.end());
        key_id_str.erase(std::remove(key_id_str.begin(), key_id_str.end(), ' '), key_id_str.end());
        char_str.erase(std::remove(char_str.begin(), char_str.end(), '"'), char_str.end());
        char_str.erase(std::remove(char_str.begin(), char_str.end(), ','), char_str.end());
        char_str.erase(std::remove(char_str.begin(), char_str.end(), ' '), char_str.end());
        
        if (!key_id_str.empty() && !char_str.empty()) {
            int key_id = std::stoi(key_id_str);
            char character = char_str[0];
            
            layout.key_to_char[key_id] = character;
            layout.char_to_key[character] = key_id;
        }
    }
};

std::unordered_map<std::string, LayoutDefinition> SimpleLayoutLoader::demo_layouts_;

} // namespace layout_converter 