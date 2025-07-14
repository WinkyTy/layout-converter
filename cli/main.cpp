// Layout Converter CLI Tool
// Command-line interface for the key ID system

#include "../core/include/key_system.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

void print_usage(const char* program_name) {
    std::cout << "Layout Converter - Convert text between keyboard layouts\n\n";
    std::cout << "Usage:\n";
    std::cout << "  " << program_name << " <text> [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --from <layout>     Source layout (qwerty, workman, russian)\n";
    std::cout << "  --to <layout>       Target layout (qwerty, workman, russian)\n";
    std::cout << "  --detect            Auto-detect possible layouts\n";
    std::cout << "  --help, -h          Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " \"hello\" --from qwerty --to workman\n";
    std::cout << "  " << program_name << " \"привет\" --detect\n";
    std::cout << "  " << program_name << " \"ywoo;\" --from workman --to qwerty\n\n";
    std::cout << "Available layouts:\n";
    std::cout << "  qwerty, workman, russian\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string text;
    std::string from_layout;
    std::string to_layout;
    bool detect_mode = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "--from" && i + 1 < argc) {
            from_layout = argv[++i];
        } else if (arg == "--to" && i + 1 < argc) {
            to_layout = argv[++i];
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
        layout_converter::KeyBasedLayoutLibrary library;

        if (detect_mode) {
            // Auto-detect mode
            std::cout << "Text: '" << text << "'\n\n";
            
            auto detected_layouts = library.detect_likely_layouts(text);
            if (detected_layouts.empty()) {
                std::cout << "No layouts detected for this text.\n";
            } else {
                std::cout << "Detected layouts: ";
                for (size_t i = 0; i < detected_layouts.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << detected_layouts[i];
                }
                std::cout << "\n\n";
                
                // Show possible conversions
                if (detected_layouts.size() >= 2) {
                    std::cout << "Possible conversions:\n";
                    for (size_t i = 0; i < detected_layouts.size(); ++i) {
                        for (size_t j = 0; j < detected_layouts.size(); ++j) {
                            if (i != j) {
                                std::string converted = library.convert_text(text, detected_layouts[i], detected_layouts[j]);
                                std::cout << "  " << detected_layouts[i] << " → " << detected_layouts[j] << ": '" << converted << "'\n";
                            }
                        }
                    }
                }
            }
        } else {
            // Direct conversion mode
            if (from_layout.empty()) {
                std::cerr << "Error: Source layout not specified. Use --from <layout>\n";
                return 1;
            }
            if (to_layout.empty()) {
                std::cerr << "Error: Target layout not specified. Use --to <layout>\n";
                return 1;
            }

            std::string converted = library.convert_text(text, from_layout, to_layout);
            std::cout << "'" << text << "' → '" << converted << "' (" << from_layout << " → " << to_layout << ")\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
} 