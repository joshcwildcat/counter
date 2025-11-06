# Use Ubuntu as base image
FROM ubuntu:24.04

# Install development tools
RUN apt-get update && \
    apt-get install -y build-essential gcc make cmake && \
    apt-get clean

# Set working directory
WORKDIR /workspace

# Default command
CMD ["/bin/bash"]
