#!/usr/bin/env python3
"""
Layout Converter Telegram Bot
Python wrapper for the C++ layout converter library
"""

import os
import sys
import ctypes
from typing import List, Dict, Any
from telegram import Update, InlineKeyboardButton, InlineKeyboardMarkup
from telegram.ext import Application, CommandHandler, MessageHandler, CallbackQueryHandler, filters, ContextTypes
from dotenv import load_dotenv

# Load environment variables
load_dotenv()

# Add the core library path
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'core'))

class LayoutConverterWrapper:
    """Python wrapper for the C++ layout converter library"""
    
    def __init__(self, library_path: str):
        """Initialize the wrapper with the C++ library"""
        try:
            # Load the C++ library
            self.lib = ctypes.CDLL(library_path)
            self._setup_function_signatures()
        except Exception as e:
            print(f"Failed to load C++ library: {e}")
            self.lib = None
    
    def _setup_function_signatures(self):
        """Setup function signatures for the C++ library"""
        if not self.lib:
            return
            
        # Define enums
        self.LayoutType = ctypes.c_int
        self.LAYOUT_QWERTY = 0
        self.LAYOUT_CYRILLIC = 1
        self.LAYOUT_COLEMAK = 2
        self.LAYOUT_WORKMAN = 3
        self.LAYOUT_DVORAK = 4
        self.LAYOUT_RUSSIAN_TYPEWRITER = 5
        self.LAYOUT_UNKNOWN = 6
        
        # Define structures
        class ConversionResult(ctypes.Structure):
            _fields_ = [
                ("original_text", ctypes.c_char_p),
                ("converted_text", ctypes.c_char_p),
                ("from_layout", self.LayoutType),
                ("to_layout", self.LayoutType),
                ("confidence", ctypes.c_double)
            ]
        
        self.ConversionResult = ConversionResult
        
        # Setup function signatures
        self.lib.layout_converter_create.restype = ctypes.c_void_p
        self.lib.layout_converter_destroy.argtypes = [ctypes.c_void_p]
        self.lib.layout_converter_convert.argtypes = [
            ctypes.c_void_p, ctypes.c_char_p, self.LayoutType, self.LayoutType
        ]
        self.lib.layout_converter_convert.restype = ctypes.POINTER(ConversionResult)
    
    def convert(self, text: str, from_layout: str, to_layout: str) -> Dict[str, Any]:
        """Convert text between layouts"""
        if not self.lib:
            return {"error": "Library not loaded"}
        
        # Convert layout strings to enum values
        from_enum = self._string_to_layout_enum(from_layout)
        to_enum = self._string_to_layout_enum(to_layout)
        
        if from_enum == self.LAYOUT_UNKNOWN or to_enum == self.LAYOUT_UNKNOWN:
            return {"error": "Invalid layout type"}
        
        # Create converter instance
        converter = self.lib.layout_converter_create()
        if not converter:
            return {"error": "Failed to create converter"}
        
        try:
            # Convert text
            result_ptr = self.lib.layout_converter_convert(
                converter, 
                text.encode('utf-8'), 
                from_enum, 
                to_enum
            )
            
            if result_ptr:
                result = result_ptr.contents
                return {
                    "original_text": result.original_text.decode('utf-8') if result.original_text else "",
                    "converted_text": result.converted_text.decode('utf-8') if result.converted_text else "",
                    "from_layout": from_layout,
                    "to_layout": to_layout,
                    "confidence": result.confidence
                }
            else:
                return {"error": "Conversion failed"}
                
        finally:
            self.lib.layout_converter_destroy(converter)
    
    def _string_to_layout_enum(self, layout_str: str) -> int:
        """Convert layout string to enum value"""
        layout_map = {
            "qwerty": self.LAYOUT_QWERTY,
            "cyrillic": self.LAYOUT_CYRILLIC,
            "colemak": self.LAYOUT_COLEMAK,
            "workman": self.LAYOUT_WORKMAN,
            "dvorak": self.LAYOUT_DVORAK,
            "russian_typewriter": self.LAYOUT_RUSSIAN_TYPEWRITER
        }
        return layout_map.get(layout_str.lower(), self.LAYOUT_UNKNOWN)

class LayoutConverterBot:
    """Telegram bot for layout conversion"""
    
    def __init__(self):
        """Initialize the bot"""
        self.token = os.getenv('TELEGRAM_BOT_TOKEN')
        if not self.token:
            raise ValueError("TELEGRAM_BOT_TOKEN not set in environment")
        
        # Initialize the C++ wrapper
        library_path = os.path.join(
            os.path.dirname(__file__), 
            '..', 
            'build', 
            'lib', 
            'liblayout_converter_core.so'
        )
        self.converter = LayoutConverterWrapper(library_path)
        
        # Available layouts
        self.layouts = ["qwerty", "cyrillic", "workman", "colemak", "dvorak"]
    
    async def start(self, update: Update, context: ContextTypes.DEFAULT_TYPE):
        """Handle /start command"""
        welcome_text = (
            "🎹 Welcome to Layout Converter Bot!\n\n"
            "I can convert text between different keyboard layouts.\n\n"
            "📝 Usage:\n"
            "• Send me text to auto-detect and convert\n"
            "• Use /convert <text> for manual conversion\n"
            "• Use /layouts to see available layouts\n\n"
            "Example: Send 'влььд' to see possible conversions!"
        )
        await update.message.reply_text(welcome_text)
    
    async def help_command(self, update: Update, context: ContextTypes.DEFAULT_TYPE):
        """Handle /help command"""
        help_text = (
            "🔧 Layout Converter Bot Help\n\n"
            "Commands:\n"
            "/start - Start the bot\n"
            "/help - Show this help\n"
            "/convert <text> - Convert text manually\n"
            "/layouts - Show available layouts\n\n"
            "Examples:\n"
            "• Send: влььд\n"
            "• /convert hello --from qwerty --to workman\n"
            "• /convert привет --from cyrillic --to qwerty"
        )
        await update.message.reply_text(help_text)
    
    async def layouts_command(self, update: Update, context: ContextTypes.DEFAULT_TYPE):
        """Handle /layouts command"""
        layouts_text = "🎹 Available Layouts:\n\n"
        for layout in self.layouts:
            layouts_text += f"• {layout.title()}\n"
        layouts_text += "\nSend me text to see possible conversions!"
        await update.message.reply_text(layouts_text)
    
    async def convert_command(self, update: Update, context: ContextTypes.DEFAULT_TYPE):
        """Handle /convert command"""
        if not context.args:
            await update.message.reply_text(
                "Usage: /convert <text> [--from <layout>] [--to <layout>]\n"
                "Example: /convert hello --from qwerty --to workman"
            )
            return
        
        # Parse arguments (simplified)
        text = context.args[0]
        from_layout = "qwerty"  # Default
        to_layout = "workman"   # Default
        
        # Simple argument parsing
        for i, arg in enumerate(context.args[1:], 1):
            if arg == "--from" and i + 1 < len(context.args):
                from_layout = context.args[i + 1]
            elif arg == "--to" and i + 1 < len(context.args):
                to_layout = context.args[i + 1]
        
        result = self.converter.convert(text, from_layout, to_layout)
        
        if "error" in result:
            await update.message.reply_text(f"❌ Error: {result['error']}")
        else:
            response = (
                f"🔄 Conversion Result:\n\n"
                f"📝 Original: {result['original_text']}\n"
                f"✅ Converted: {result['converted_text']}\n"
                f"🔄 {result['from_layout'].title()} → {result['to_layout'].title()}\n"
                f"📊 Confidence: {result['confidence']:.1%}"
            )
            await update.message.reply_text(response)
    
    async def handle_message(self, update: Update, context: ContextTypes.DEFAULT_TYPE):
        """Handle incoming text messages"""
        text = update.message.text
        
        if not text or text.startswith('/'):
            return
        
        # For now, show a simple response since we don't have full detection
        response = (
            f"📝 Received: {text}\n\n"
            f"🔄 Try these conversions:\n"
            f"• /convert {text} --from qwerty --to workman\n"
            f"• /convert {text} --from cyrillic --to qwerty\n"
            f"• /convert {text} --from workman --to qwerty\n\n"
            f"💡 Tip: Use /convert command for specific conversions!"
        )
        await update.message.reply_text(response)
    
    def run(self):
        """Run the bot"""
        # Create application
        application = Application.builder().token(self.token).build()
        
        # Add handlers
        application.add_handler(CommandHandler("start", self.start))
        application.add_handler(CommandHandler("help", self.help_command))
        application.add_handler(CommandHandler("layouts", self.layouts_command))
        application.add_handler(CommandHandler("convert", self.convert_command))
        application.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, self.handle_message))
        
        # Start the bot
        print("🤖 Starting Layout Converter Bot...")
        application.run_polling()

if __name__ == "__main__":
    try:
        bot = LayoutConverterBot()
        bot.run()
    except Exception as e:
        print(f"❌ Failed to start bot: {e}")
        sys.exit(1) 