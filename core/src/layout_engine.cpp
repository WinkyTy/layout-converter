// Layout Engine Implementation
// Handles core conversion logic between different keyboard layouts

#include "../include/layout_converter.h"
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

namespace layout_converter {

// Layout mapping data
namespace {
    // QWERTY to other layouts mappings (using ASCII for now)
    const std::unordered_map<char, char> qwerty_to_workman = {
        {'q', 'd'}, {'w', 'r'}, {'e', 'w'}, {'r', 'b'}, {'t', 'j'}, {'y', 'f'}, {'u', 'u'}, {'i', 'p'}, {'o', ';'}, {'p', 'l'},
        {'a', 'a'}, {'s', 's'}, {'d', 'h'}, {'f', 't'}, {'g', 'g'}, {'h', 'y'}, {'j', 'n'}, {'k', 'e'}, {'l', 'o'},
        {'z', 'z'}, {'x', 'x'}, {'c', 'm'}, {'v', 'c'}, {'b', 'v'}, {'n', 'k'}, {'m', 'l'}
    };

    const std::unordered_map<char, char> qwerty_to_colemak = {
        {'q', 'q'}, {'w', 'w'}, {'e', 'f'}, {'r', 'p'}, {'t', 'g'}, {'y', 'j'}, {'u', 'l'}, {'i', 'u'}, {'o', 'y'}, {'p', ';'},
        {'a', 'a'}, {'s', 'r'}, {'d', 's'}, {'f', 't'}, {'g', 'd'}, {'h', 'h'}, {'j', 'n'}, {'k', 'e'}, {'l', 'i'},
        {'z', 'z'}, {'x', 'x'}, {'c', 'c'}, {'v', 'v'}, {'b', 'b'}, {'n', 'k'}, {'m', 'm'}
    };

    const std::unordered_map<char, char> qwerty_to_dvorak = {
        {'q', '\''}, {'w', ','}, {'e', '.'}, {'r', 'p'}, {'t', 'y'}, {'y', 'f'}, {'u', 'g'}, {'i', 'c'}, {'o', 'r'}, {'p', 'l'},
        {'a', 'a'}, {'s', 'o'}, {'d', 'e'}, {'f', 'u'}, {'g', 'i'}, {'h', 'd'}, {'j', 'h'}, {'k', 't'}, {'l', 'n'},
        {'z', ';'}, {'x', 'q'}, {'c', 'j'}, {'v', 'k'}, {'b', 'x'}, {'n', 'b'}, {'m', 'm'}
    };

    // Simplified Cyrillic mapping (using transliteration for now)
    const std::unordered_map<char, char> qwerty_to_cyrillic_simple = {
        {'q', 'q'}, {'w', 'w'}, {'e', 'e'}, {'r', 'r'}, {'t', 't'}, {'y', 'y'}, {'u', 'u'}, {'i', 'i'}, {'o', 'o'}, {'p', 'p'},
        {'a', 'a'}, {'s', 's'}, {'d', 'd'}, {'f', 'f'}, {'g', 'g'}, {'h', 'h'}, {'j', 'j'}, {'k', 'k'}, {'l', 'l'},
        {'z', 'z'}, {'x', 'x'}, {'c', 'c'}, {'v', 'v'}, {'b', 'b'}, {'n', 'n'}, {'m', 'm'}
    };

    // Helper function to create reverse mappings
    template<typename Map>
    Map create_reverse_map(const Map& original) {
        Map reverse;
        for (const auto& pair : original) {
            reverse[pair.second] = pair.first;
        }
        return reverse;
    }

    // Create reverse mappings
    const auto workman_to_qwerty = create_reverse_map(qwerty_to_workman);
    const auto colemak_to_qwerty = create_reverse_map(qwerty_to_colemak);
    const auto dvorak_to_qwerty = create_reverse_map(qwerty_to_dvorak);
}

// LayoutConverter implementation
class LayoutConverter::Impl {
public:
    Impl() = default;
    ~Impl() = default;

    ConversionResult convert(const std::string& text, LayoutType from, LayoutType to) {
        ConversionResult result;
        result.original_text = text;
        result.from_layout = from;
        result.to_layout = to;
        result.converted_text = convert_text(text, from, to);
        result.confidence = 1.0; // For now, assume 100% confidence
        
        return result;
    }

    DetectionResult detect_and_convert(const std::string& text) {
        DetectionResult result;
        result.text = text;
        
        // For now, try common conversions
        std::vector<LayoutType> common_layouts = {
            LayoutType::QWERTY, LayoutType::CYRILLIC, 
            LayoutType::WORKMAN, LayoutType::COLEMAK, LayoutType::DVORAK
        };
        
        for (LayoutType from : common_layouts) {
            for (LayoutType to : common_layouts) {
                if (from != to) {
                    ConversionResult conv = convert(text, from, to);
                    if (conv.confidence > 0.5) { // Simple threshold
                        result.possible_conversions.push_back(conv);
                    }
                }
            }
        }
        
        // Sort by confidence
        std::sort(result.possible_conversions.begin(), result.possible_conversions.end(),
                  [](const ConversionResult& a, const ConversionResult& b) {
                      return a.confidence > b.confidence;
                  });
        
        return result;
    }

private:
    std::string convert_text(const std::string& text, LayoutType from, LayoutType to) {
        if (from == to) {
            return text;
        }

        std::string result;
        result.reserve(text.length());

        for (char c : text) {
            char converted = convert_char(c, from, to);
            result += converted;
        }

        return result;
    }

    char convert_char(char c, LayoutType from, LayoutType to) {
        // Convert to lowercase for mapping
        char lower_c = std::tolower(c);
        bool was_upper = std::isupper(c);

        char converted = c; // Default to original character

        // Handle different layout conversions
        if (from == LayoutType::QWERTY) {
            converted = convert_from_qwerty(lower_c, to);
        } else if (from == LayoutType::CYRILLIC) {
            converted = convert_from_cyrillic(lower_c, to);
        } else if (from == LayoutType::WORKMAN) {
            converted = convert_from_workman(lower_c, to);
        } else if (from == LayoutType::COLEMAK) {
            converted = convert_from_colemak(lower_c, to);
        } else if (from == LayoutType::DVORAK) {
            converted = convert_from_dvorak(lower_c, to);
        }

        // Preserve case
        return was_upper ? std::toupper(converted) : converted;
    }

    char convert_from_qwerty(char c, LayoutType to) {
        switch (to) {
            case LayoutType::WORKMAN: {
                auto it = qwerty_to_workman.find(c);
                return it != qwerty_to_workman.end() ? it->second : c;
            }
            case LayoutType::COLEMAK: {
                auto it = qwerty_to_colemak.find(c);
                return it != qwerty_to_colemak.end() ? it->second : c;
            }
            case LayoutType::DVORAK: {
                auto it = qwerty_to_dvorak.find(c);
                return it != qwerty_to_dvorak.end() ? it->second : c;
            }
            case LayoutType::CYRILLIC: {
                auto it = qwerty_to_cyrillic_simple.find(c);
                return it != qwerty_to_cyrillic_simple.end() ? it->second : c;
            }
            default:
                return c;
        }
    }

    char convert_from_cyrillic(char c, LayoutType to) {
        // For now, treat Cyrillic as QWERTY for simplicity
        return convert_from_qwerty(c, to);
    }

    char convert_from_workman(char c, LayoutType to) {
        // Convert workman to qwerty first, then to target
        char qwerty_char = convert_from_qwerty(c, LayoutType::QWERTY);
        return convert_from_qwerty(qwerty_char, to);
    }

    char convert_from_colemak(char c, LayoutType to) {
        // Convert colemak to qwerty first, then to target
        char qwerty_char = convert_from_qwerty(c, LayoutType::QWERTY);
        return convert_from_qwerty(qwerty_char, to);
    }

    char convert_from_dvorak(char c, LayoutType to) {
        // Convert dvorak to qwerty first, then to target
        char qwerty_char = convert_from_qwerty(c, LayoutType::QWERTY);
        return convert_from_qwerty(qwerty_char, to);
    }
};

// LayoutConverter implementation
LayoutConverter::LayoutConverter() : pImpl(std::make_unique<Impl>()) {}
LayoutConverter::~LayoutConverter() = default;

LayoutConverter::LayoutConverter(LayoutConverter&&) noexcept = default;
LayoutConverter& LayoutConverter::operator=(LayoutConverter&&) noexcept = default;

ConversionResult LayoutConverter::convert(const std::string& text, LayoutType from, LayoutType to) {
    return pImpl->convert(text, from, to);
}

DetectionResult LayoutConverter::detect_and_convert(const std::string& text) {
    return pImpl->detect_and_convert(text);
}

// Utility functions
std::string layout_type_to_string(LayoutType type) {
    switch (type) {
        case LayoutType::QWERTY: return "QWERTY";
        case LayoutType::CYRILLIC: return "Cyrillic";
        case LayoutType::COLEMAK: return "Colemak";
        case LayoutType::WORKMAN: return "Workman";
        case LayoutType::DVORAK: return "Dvorak";
        case LayoutType::RUSSIAN_TYPEWRITER: return "Russian Typewriter";
        case LayoutType::UNKNOWN: return "Unknown";
        default: return "Unknown";
    }
}

LayoutType string_to_layout_type(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower == "qwerty") return LayoutType::QWERTY;
    if (lower == "cyrillic") return LayoutType::CYRILLIC;
    if (lower == "colemak") return LayoutType::COLEMAK;
    if (lower == "workman") return LayoutType::WORKMAN;
    if (lower == "dvorak") return LayoutType::DVORAK;
    if (lower == "russian_typewriter") return LayoutType::RUSSIAN_TYPEWRITER;
    
    return LayoutType::UNKNOWN;
}

bool is_valid_layout_type(LayoutType type) {
    return type != LayoutType::UNKNOWN;
}

// Exception implementation
LayoutConverterException::LayoutConverterException(const std::string& message) 
    : message_(message) {}

const char* LayoutConverterException::what() const noexcept {
    return message_.c_str();
}

} // namespace layout_converter 