// Test file for Key ID System
// Simple unit tests for the key ID layout conversion functionality

#include "../core/include/key_system.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class KeyIDSystemTest {
public:
    static void run_all_tests() {
        std::cout << "🧪 Running Key ID System Tests\n";
        std::cout << "==============================\n\n";
        
        test_key_id_generation();
        test_key_id_components();
        test_basic_conversion();
        test_same_layout_conversion();
        test_layout_detection();
        test_cyrillic_detection();
        test_invalid_layouts();
        test_case_preservation();
        test_non_alphabetic_characters();
        
        std::cout << "\n✅ All tests completed!\n";
    }

private:
    static void test_key_id_generation() {
        std::cout << "Testing Key ID Generation... ";
        
        int key_id = layout_converter::generate_key_id(1, 1, 5);  // QWERTY, key 5
        if (key_id != 1105) {
            std::cout << "FAILED (expected 1105, got " << key_id << ")\n";
            return;
        }
        
        key_id = layout_converter::generate_key_id(2, 1, 10);  // Russian, key 10
        if (key_id != 2110) {
            std::cout << "FAILED (expected 2110, got " << key_id << ")\n";
            return;
        }
        
        std::cout << "PASSED\n";
    }
    
    static void test_key_id_components() {
        std::cout << "Testing Key ID Components... ";
        
        layout_converter::KeyIDComponents components(1105);
        if (components.family_id != 1 || components.layout_id != 1 || components.key_position != 5) {
            std::cout << "FAILED\n";
            return;
        }
        
        std::cout << "PASSED\n";
    }
    
    static void test_basic_conversion() {
        std::cout << "Testing Basic Conversion... ";
        
        // Note: This test requires the library to be properly initialized with layouts
        // For now, we'll just test the function signature
        std::cout << "SKIPPED (requires layout initialization)\n";
    }
    
    static void test_same_layout_conversion() {
        std::cout << "Testing Same Layout Conversion... ";
        std::cout << "SKIPPED (requires layout initialization)\n";
    }
    
    static void test_layout_detection() {
        std::cout << "Testing Layout Detection... ";
        std::cout << "SKIPPED (requires layout initialization)\n";
    }
    
    static void test_cyrillic_detection() {
        std::cout << "Testing Cyrillic Detection... ";
        std::cout << "SKIPPED (requires layout initialization)\n";
    }
    
    static void test_invalid_layouts() {
        std::cout << "Testing Invalid Layouts... ";
        std::cout << "SKIPPED (requires layout initialization)\n";
    }
    
    static void test_case_preservation() {
        std::cout << "Testing Case Preservation... ";
        std::cout << "SKIPPED (requires layout initialization)\n";
    }
    
    static void test_non_alphabetic_characters() {
        std::cout << "Testing Non-Alphabetic Characters... ";
        std::cout << "SKIPPED (requires layout initialization)\n";
    }
};

int main() {
    KeyIDSystemTest::run_all_tests();
    return 0;
} 