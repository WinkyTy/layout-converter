# Layout Converter

A high-performance, multi-platform layout conversion tool that can detect and convert text between different keyboard layouts.

## Features

- **Multi-layout Support**: QWERTY, Cyrillic, Colemak, Workman, Russian Typewriter, Dvorak, and more
- **Smart Detection**: AI-powered layout prediction with probability scoring
- **Cross-platform**: Windows, macOS, Linux, iOS, Android
- **Multiple Interfaces**: CLI, Telegram Bot, Web UI, Mobile Apps
- **High Performance**: C++ core engine for optimal speed

## Architecture

```
layout_converter/
├── core/                    # C++ core library
│   ├── src/
│   │   ├── layout_engine.cpp
│   │   ├── layout_detector.cpp
│   │   ├── probability_scorer.cpp
│   │   └── layouts/
│   │       ├── qwerty.cpp
│   │       ├── cyrillic.cpp
│   │       ├── colemak.cpp
│   │       └── workman.cpp
│   ├── include/
│   └── CMakeLists.txt
├── cli/                     # Command-line interface
├── telegram_bot/            # Python Telegram bot
├── web_ui/                  # Local web interface
├── mobile/                  # Mobile app wrappers
└── tests/                   # Test suite
```

## Development Phases

### Phase 1: Core Engine (C++)
- [ ] Layout mapping system
- [ ] Basic conversion algorithms
- [ ] Layout detection foundation
- [ ] Shared library interface

### Phase 2: CLI Tool
- [ ] Command-line interface
- [ ] File I/O support
- [ ] Batch processing

### Phase 3: Telegram Bot
- [ ] Python wrapper for C++ core
- [ ] Telegram bot implementation
- [ ] Inline keyboard for layout selection

### Phase 4: Web Interface
- [ ] Local web server
- [ ] Modern UI with real-time conversion
- [ ] Layout probability display

### Phase 5: Advanced Features
- [ ] Machine learning for layout prediction
- [ ] Custom layout definitions
- [ ] Performance optimization

## Technology Stack

- **Core**: C++17/20
- **Build System**: CMake
- **Telegram Bot**: Python + python-telegram-bot
- **Web UI**: Python Flask/FastAPI + HTML/CSS/JS
- **Testing**: Google Test (C++), pytest (Python)
- **Documentation**: Doxygen (C++), Sphinx (Python)

## Getting Started

### Prerequisites
- C++17 compatible compiler
- CMake 3.15+
- Python 3.8+
- Git

### Building
```bash
mkdir build && cd build
cmake ..
make
```

## Usage Examples

### CLI
```bash
./layout_converter "влььд" --from cyrillic --to workman
# Output: hello

./layout_converter "влььд" --detect
# Output: 
# Possible layouts (sorted by probability):
# 1. Cyrillic -> Workman: hello (95%)
# 2. Cyrillic -> QWERTY: hello (85%)
# 3. Cyrillic -> Colemak: hello (80%)
```

### Telegram Bot
```
User: влььд
Bot: Detected possible conversions:
1. Cyrillic → Workman: hello (95%)
2. Cyrillic → QWERTY: hello (85%)
3. Cyrillic → Colemak: hello (80%)
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass
5. Submit a pull request

## License

MIT License - see LICENSE file for details 