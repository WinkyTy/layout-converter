#!/usr/bin/env python3
"""
Health check script for Railway deployment
"""

import os
import sys

def check_health():
    """Check if the bot is healthy"""
    try:
        # Check if required environment variables are set
        token = os.getenv('TELEGRAM_BOT_TOKEN')
        if not token:
            print("ERROR: TELEGRAM_BOT_TOKEN not set")
            return False
        
        # Check if we can import required modules
        try:
            from telegram import Update
            from telegram.ext import Application
            print("SUCCESS: Telegram modules imported")
        except ImportError as e:
            print(f"ERROR: Failed to import Telegram modules: {e}")
            return False
        
        # Check if C++ library exists (optional)
        library_paths = [
            os.path.join(os.path.dirname(__file__), '..', 'build', 'lib', 'liblayout_converter_core.so'),
            os.path.join(os.path.dirname(__file__), '..', 'build', 'lib', 'liblayout_converter_core.dylib'),
            '/app/build/lib/liblayout_converter_core.so',
            '/app/build/lib/liblayout_converter_core.dylib'
        ]
        
        library_found = any(os.path.exists(path) for path in library_paths)
        if library_found:
            print("SUCCESS: C++ library found")
        else:
            print("WARNING: C++ library not found, will use fallback mode")
        
        print("SUCCESS: Health check passed")
        return True
        
    except Exception as e:
        print(f"ERROR: Health check failed: {e}")
        return False

if __name__ == "__main__":
    success = check_health()
    sys.exit(0 if success else 1) 