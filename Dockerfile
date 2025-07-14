# Use Ubuntu as base image for C++ compilation
FROM ubuntu:20.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV PYTHONUNBUFFERED=1

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    python3-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy the entire project
COPY . .

# Build the C++ library
RUN chmod +x build.sh && ./build.sh

# Install Python dependencies
RUN pip3 install -r telegram_bot/requirements.txt

# Create a script to run the bot
RUN echo '#!/bin/bash\ncd /app/telegram_bot\npython3 layout_converter_bot.py' > /app/run_bot.sh
RUN chmod +x /app/run_bot.sh

# Expose port (Railway will handle this)
EXPOSE 8000

# Run the bot
CMD ["/app/run_bot.sh"] 