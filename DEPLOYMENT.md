# Layout Converter Deployment Guide

This guide covers how to deploy the Layout Converter project on different platforms.

## Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15+
- Python 3.8+ (for Telegram bot)
- Git

## Building the Core Library

### Linux/macOS

```bash
# Clone the repository
git clone <your-repo-url>
cd layout_converter

# Build the project
./build.sh

# Or manually:
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Windows

```bash
# Using Visual Studio
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release

# Using MinGW
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

## CLI Tool Usage

After building, you can use the CLI tool:

```bash
# Basic conversion
./build/bin/layout_converter "hello" --from qwerty --to workman

# Auto-detection
./build/bin/layout_converter "влььд" --detect

# Help
./build/bin/layout_converter --help
```

## Telegram Bot Deployment

### 1. Create a Telegram Bot

1. Message [@BotFather](https://t.me/botfather) on Telegram
2. Send `/newbot` command
3. Follow the instructions to create your bot
4. Save the bot token

### 2. Set Up Environment

```bash
cd telegram_bot

# Install Python dependencies
pip install -r requirements.txt

# Create .env file
echo "TELEGRAM_BOT_TOKEN=your_bot_token_here" > .env
```

### 3. Run the Bot

```bash
# Make sure the C++ library is built first
cd ..
./build.sh

# Run the bot
cd telegram_bot
python layout_converter_bot.py
```

### 4. Deploy to Cloud (Optional)

#### Railway
1. Create account on [Railway](https://railway.app)
2. Connect your GitHub repository
3. Set environment variable `TELEGRAM_BOT_TOKEN`
4. Deploy

#### Heroku
1. Create `Procfile`:
```
worker: python telegram_bot/layout_converter_bot.py
```

2. Deploy using Heroku CLI or GitHub integration

## Mobile App Development

### React Native
```bash
# Create new React Native project
npx react-native init LayoutConverterApp

# Add native module for C++ library
# Use react-native-ffi or similar library
```

### Flutter
```bash
# Create new Flutter project
flutter create layout_converter_app

# Add FFI support for C++ library
# Use dart:ffi for native library calls
```

## Web Interface

### Local Development
```bash
# Create web interface directory
mkdir web_ui
cd web_ui

# Create simple Flask app
# (See web_ui/app.py for implementation)
```

### Deployment
- **Vercel**: Deploy static files
- **Netlify**: Deploy static files
- **Railway**: Deploy Flask app

## Cross-Platform Distribution

### macOS
```bash
# Create .app bundle
mkdir -p LayoutConverter.app/Contents/MacOS
cp build/bin/layout_converter LayoutConverter.app/Contents/MacOS/
# Add Info.plist and other resources
```

### Windows
```bash
# Create installer using NSIS or similar
# Package with required DLLs
```

### Linux
```bash
# Create .deb package
# Package with dependencies
```

## Docker Deployment

### Dockerfile
```dockerfile
FROM ubuntu:20.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    python3 \
    python3-pip

# Copy source code
COPY . /app
WORKDIR /app

# Build C++ library
RUN ./build.sh

# Install Python dependencies
RUN pip3 install -r telegram_bot/requirements.txt

# Run bot
CMD ["python3", "telegram_bot/layout_converter_bot.py"]
```

### Docker Compose
```yaml
version: '3.8'
services:
  layout-converter-bot:
    build: .
    environment:
      - TELEGRAM_BOT_TOKEN=${TELEGRAM_BOT_TOKEN}
    restart: unless-stopped
```

## Performance Optimization

### C++ Library
- Use `-O3` optimization flags
- Enable link-time optimization (LTO)
- Profile with tools like `perf` or `gprof`

### Python Wrapper
- Use `pybind11` instead of `ctypes` for better performance
- Implement caching for frequently used conversions
- Use async/await for I/O operations

## Monitoring and Logging

### Telegram Bot
```python
import logging

logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s'
)
```

### Metrics
- Track conversion requests
- Monitor response times
- Log errors and exceptions

## Security Considerations

1. **Bot Token**: Keep bot token secure, never commit to version control
2. **Input Validation**: Validate all user inputs
3. **Rate Limiting**: Implement rate limiting for API calls
4. **Error Handling**: Don't expose internal errors to users

## Troubleshooting

### Common Issues

1. **Library not found**: Ensure C++ library is built and in PATH
2. **Bot not responding**: Check bot token and network connectivity
3. **Build failures**: Verify compiler and CMake versions
4. **UTF-8 issues**: Ensure proper encoding for Cyrillic characters

### Debug Mode
```bash
# Enable debug logging
export DEBUG=1
python telegram_bot/layout_converter_bot.py
```

## Future Enhancements

1. **Machine Learning**: Implement layout detection using ML
2. **Custom Layouts**: Allow users to define custom layouts
3. **Batch Processing**: Support for file uploads and batch conversions
4. **API**: Create REST API for third-party integrations
5. **Mobile Apps**: Native iOS and Android applications 