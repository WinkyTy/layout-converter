// Layout Converter CLI Tool
// Command-line interface for the layout converter library

#include "../core/include/layout_converter.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

void print_usage(const char* program_name) {
    std::cout << "Layout Converter - Convert text between keyboard layouts\n\n";
    std::cout << "Usage:\n";
    std::cout << "  " << program_name << " <text> [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --from <layout>     Source layout (qwerty, cyrillic, workman, colemak, dvorak)\n";
    std::cout << "  --to <layout>       Target layout (qwerty, cyrillic, workman, colemak, dvorak)\n";
    std::cout << "  --detect            Auto-detect possible conversions\n";
    std::cout << "  --help, -h          Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " \"влььд\" --from cyrillic --to workman\n";
    std::cout << "  " << program_name << " \"влььд\" --detect\n";
    std::cout << "  " << program_name << " \"hello\" --from qwerty --to dvorak\n\n";
    std::cout << "Available layouts:\n";
    std::cout << "  qwerty, cyrillic, workman, colemak, dvorak, russian_typewriter\n";
}

void print_conversion_result(const layout_converter::ConversionResult& result) {
    std::cout << result.original_text << " → " << result.converted_text;
    std::cout << " (" << layout_converter::layout_type_to_string(result.from_layout);
    std::cout << " → " << layout_converter::layout_type_to_string(result.to_layout) << ")";
    if (result.confidence < 1.0) {
        std::cout << " [" << static_cast<int>(result.confidence * 100) << "%]";
    }
    std::cout << "\n";
}

void print_detection_results(const layout_converter::DetectionResult& result) {
    std::cout << "Text: " << result.text << "\n\n";
    std::cout << "Possible conversions (sorted by probability):\n";
    
    for (size_t i = 0; i < result.possible_conversions.size(); ++i) {
        const auto& conv = result.possible_conversions[i];
        std::cout << i + 1 << ". ";
        print_conversion_result(conv);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string text;
    layout_converter::LayoutType from_layout = layout_converter::LayoutType::UNKNOWN;
    layout_converter::LayoutType to_layout = layout_converter::LayoutType::UNKNOWN;
    bool detect_mode = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "--from" && i + 1 < argc) {
            from_layout = layout_converter::string_to_layout_type(argv[++i]);
        } else if (arg == "--to" && i + 1 < argc) {
            to_layout = layout_converter::string_to_layout_type(argv[++i]);
        } else if (arg == "--detect") {
            detect_mode = true;
        } else if (text.empty()) {
            text = arg;
        } else {
            std::cerr << "Error: Unknown argument '" << arg << "'\n";
            print_usage(argv[0]);
            return 1;
        }
    }

    if (text.empty()) {
        std::cerr << "Error: No text provided\n";
        print_usage(argv[0]);
        return 1;
    }

    try {
        layout_converter::LayoutConverter converter;

        if (detect_mode) {
            // Auto-detect mode
            auto result = converter.detect_and_convert(text);
            print_detection_results(result);
        } else {
            // Direct conversion mode
            if (from_layout == layout_converter::LayoutType::UNKNOWN) {
                std::cerr << "Error: Source layout not specified. Use --from <layout>\n";
                return 1;
            }
            if (to_layout == layout_converter::LayoutType::UNKNOWN) {
                std::cerr << "Error: Target layout not specified. Use --to <layout>\n";
                return 1;
            }

            auto result = converter.convert(text, from_layout, to_layout);
            print_conversion_result(result);
        }

    } catch (const layout_converter::LayoutConverterException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }

    return 0;
} 