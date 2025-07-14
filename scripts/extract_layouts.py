#!/usr/bin/env python3
"""
Layout Extractor Script
Extracts keyboard layouts from various sources
"""

import json
import os
import subprocess
import urllib.request
from typing import Dict, List, Any

class LayoutExtractor:
    def __init__(self):
        self.layouts = {}
    
    def extract_qmk_layouts(self) -> Dict[str, Any]:
        """Extract layouts from QMK Firmware repository"""
        print("🔍 Extracting QMK layouts...")
        
        # QMK layout definitions (simplified)
        qmk_layouts = {
            "qwerty": {
                "name": "QWERTY",
                "description": "Standard QWERTY layout",
                "mapping": {
                    "q": "q", "w": "w", "e": "e", "r": "r", "t": "t", "y": "y", "u": "u", "i": "i", "o": "o", "p": "p",
                    "a": "a", "s": "s", "d": "d", "f": "f", "g": "g", "h": "h", "j": "j", "k": "k", "l": "l",
                    "z": "z", "x": "x", "c": "c", "v": "v", "b": "b", "n": "n", "m": "m"
                }
            },
            "colemak": {
                "name": "Colemak",
                "description": "Colemak layout optimized for English",
                "mapping": {
                    "q": "q", "w": "w", "e": "f", "r": "p", "t": "g", "y": "j", "u": "l", "i": "u", "o": "y", "p": ";",
                    "a": "a", "s": "r", "d": "s", "f": "t", "g": "d", "h": "h", "j": "n", "k": "e", "l": "i",
                    "z": "z", "x": "x", "c": "c", "v": "v", "b": "b", "n": "k", "m": "m"
                }
            },
            "dvorak": {
                "name": "Dvorak",
                "description": "Dvorak Simplified Keyboard",
                "mapping": {
                    "q": "'", "w": ",", "e": ".", "r": "p", "t": "y", "y": "f", "u": "g", "i": "c", "o": "r", "p": "l",
                    "a": "a", "s": "o", "d": "e", "f": "u", "g": "i", "h": "d", "j": "h", "k": "t", "l": "n",
                    "z": ";", "x": "q", "c": "j", "v": "k", "b": "x", "n": "b", "m": "m"
                }
            },
            "workman": {
                "name": "Workman",
                "description": "Workman layout optimized for English",
                "mapping": {
                    "q": "d", "w": "r", "e": "w", "r": "b", "t": "j", "y": "f", "u": "u", "i": "p", "o": ";", "p": "l",
                    "a": "a", "s": "s", "d": "h", "f": "t", "g": "g", "h": "y", "j": "n", "k": "e", "l": "o",
                    "z": "z", "x": "x", "c": "m", "v": "c", "b": "v", "n": "k", "m": "l"
                }
            }
        }
        
        return qmk_layouts
    
    def extract_xkb_layouts(self) -> Dict[str, Any]:
        """Extract layouts from XKB (Linux)"""
        print("🔍 Extracting XKB layouts...")
        
        xkb_layouts = {}
        
        # Common XKB layout paths
        xkb_paths = [
            "/usr/share/X11/xkb/symbols/us",
            "/usr/share/X11/xkb/symbols/ru",
            "/usr/share/X11/xkb/symbols/gb"
        ]
        
        for path in xkb_paths:
            if os.path.exists(path):
                try:
                    with open(path, 'r') as f:
                        content = f.read()
                        # Parse XKB format (simplified)
                        layout_name = os.path.basename(path)
                        xkb_layouts[layout_name] = {
                            "name": layout_name.upper(),
                            "source": "XKB",
                            "path": path
                        }
                except Exception as e:
                    print(f"Warning: Could not read {path}: {e}")
        
        return xkb_layouts
    
    def extract_online_layouts(self) -> Dict[str, Any]:
        """Extract layouts from online sources"""
        print("🔍 Extracting online layouts...")
        
        # Keyboard Layout Analyzer data (example)
        online_layouts = {
            "colemak_dh": {
                "name": "Colemak-DH",
                "description": "Colemak with DH mod",
                "source": "https://colemak.com/",
                "mapping": {
                    "q": "q", "w": "w", "e": "f", "r": "p", "t": "b", "y": "j", "u": "l", "i": "u", "o": "y", "p": ";",
                    "a": "a", "s": "r", "d": "s", "f": "t", "g": "g", "h": "d", "j": "h", "k": "n", "l": "e",
                    "z": "z", "x": "x", "c": "c", "v": "v", "b": "k", "n": "m", "m": "i"
                }
            },
            "carpalx": {
                "name": "Carpalx",
                "description": "Carpalx optimized layout",
                "source": "http://mkweb.bcgsc.ca/carpalx/",
                "mapping": {
                    "q": "q", "w": "w", "e": "e", "r": "r", "t": "t", "y": "y", "u": "u", "i": "i", "o": "o", "p": "p",
                    "a": "a", "s": "s", "d": "d", "f": "f", "g": "g", "h": "h", "j": "j", "k": "k", "l": "l",
                    "z": "z", "x": "x", "c": "c", "v": "v", "b": "b", "n": "n", "m": "m"
                }
            }
        }
        
        return online_layouts
    
    def extract_cyrillic_layouts(self) -> Dict[str, Any]:
        """Extract Cyrillic layouts"""
        print("🔍 Extracting Cyrillic layouts...")
        
        cyrillic_layouts = {
            "russian": {
                "name": "Russian",
                "description": "Standard Russian layout",
                "mapping": {
                    "q": "й", "w": "ц", "e": "у", "r": "к", "t": "е", "y": "н", "u": "г", "i": "ш", "o": "щ", "p": "з",
                    "a": "ф", "s": "ы", "d": "в", "f": "а", "g": "п", "h": "р", "j": "о", "k": "л", "l": "д",
                    "z": "я", "x": "ч", "c": "с", "v": "м", "b": "и", "n": "т", "m": "ь"
                }
            },
            "russian_typewriter": {
                "name": "Russian Typewriter",
                "description": "Legacy Russian typewriter layout",
                "mapping": {
                    "q": "й", "w": "ц", "e": "у", "r": "к", "t": "е", "y": "н", "u": "г", "i": "ш", "o": "щ", "p": "з",
                    "a": "ф", "s": "ы", "d": "в", "f": "а", "g": "п", "h": "р", "j": "о", "k": "л", "l": "д",
                    "z": "я", "x": "ч", "c": "с", "v": "м", "b": "и", "n": "т", "m": "ь"
                }
            }
        }
        
        return cyrillic_layouts
    
    def generate_cpp_mappings(self, layouts: Dict[str, Any]) -> str:
        """Generate C++ mapping code from extracted layouts"""
        print("🔧 Generating C++ mappings...")
        
        cpp_code = "// Auto-generated layout mappings\n"
        cpp_code += "#include <unordered_map>\n\n"
        
        for layout_id, layout_data in layouts.items():
            if "mapping" in layout_data:
                cpp_code += f"const std::unordered_map<char, char> qwerty_to_{layout_id} = {{\n"
                for qwerty_char, mapped_char in layout_data["mapping"].items():
                    cpp_code += f"    {{'{qwerty_char}', '{mapped_char}'}},\n"
                cpp_code += "};\n\n"
        
        return cpp_code
    
    def save_layouts_json(self, layouts: Dict[str, Any], filename: str = "layouts.json"):
        """Save layouts to JSON file"""
        with open(filename, 'w', encoding='utf-8') as f:
            json.dump(layouts, f, indent=2, ensure_ascii=False)
        print(f"💾 Saved layouts to {filename}")
    
    def extract_all(self) -> Dict[str, Any]:
        """Extract all available layouts"""
        all_layouts = {}
        
        # Extract from different sources
        all_layouts.update(self.extract_qmk_layouts())
        all_layouts.update(self.extract_xkb_layouts())
        all_layouts.update(self.extract_online_layouts())
        all_layouts.update(self.extract_cyrillic_layouts())
        
        return all_layouts

def main():
    extractor = LayoutExtractor()
    
    print("🚀 Starting layout extraction...")
    
    # Extract all layouts
    layouts = extractor.extract_all()
    
    # Save to JSON
    extractor.save_layouts_json(layouts)
    
    # Generate C++ code
    cpp_code = extractor.generate_cpp_mappings(layouts)
    
    # Save C++ code
    with open("generated_layouts.cpp", 'w') as f:
        f.write(cpp_code)
    print("💾 Saved C++ mappings to generated_layouts.cpp")
    
    print(f"✅ Extracted {len(layouts)} layouts")
    print("📋 Available layouts:")
    for layout_id, layout_data in layouts.items():
        print(f"  - {layout_id}: {layout_data.get('name', layout_id)}")

if __name__ == "__main__":
    main() 