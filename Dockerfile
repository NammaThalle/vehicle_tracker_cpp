# Base image with OpenCV
FROM murakawatakuya/opencv-in-cpp-and-python:latest

# Install build tools and any additional dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential cmake \
    # wget ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# # Install ONNX Runtime
# RUN wget https://github.com/microsoft/onnxruntime/releases/download/v1.19.0/onnxruntime-linux-x64-1.19.0.tgz && \
#     tar -xzf onnxruntime-linux-x64-1.19.0.tgz && \
#     mv onnxruntime-linux-x64-1.19.0 /opt/onnxruntime && \
#     rm onnxruntime-linux-x64-1.19.0.tgz

# Set working directory
WORKDIR /app

# Copy source code and other files
COPY src/ /app/src/
COPY models/ /app/models/
COPY input/ /app/input/
COPY CMakeLists.txt /app/

# Build C++ project
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make

# Set the entrypoint to run application
ENTRYPOINT ["./build/vehicle_tracking"]
