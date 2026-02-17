#!/bin/bash
# Script to run all headless examples and generate reference images

# Configuration
DISPLAY_NUM=99

# Set up display for offscreen rendering
export DISPLAY=${DISPLAY:-:$DISPLAY_NUM}
export COIN_GLX_PIXMAP_DIRECT_RENDERING=1

# Check if Xvfb is running, start if needed
if ! pgrep Xvfb > /dev/null; then
    echo "Starting Xvfb..."
    Xvfb :$DISPLAY_NUM -screen 0 1024x768x24 > /dev/null 2>&1 &
    XVFB_PID=$!
    sleep 2
fi

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR="$SCRIPT_DIR/build"
OUTPUT_DIR="$BUILD_DIR/output"
BIN_DIR="$BUILD_DIR/bin"

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Build if needed
if [ ! -d "$BUILD_DIR" ] || [ ! -f "$BIN_DIR/02.1.HelloCone" ]; then
    echo "Building examples..."
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    cmake .. && make -j4
    cd - > /dev/null
fi

# Run all examples
echo "Running examples..."
cd "$OUTPUT_DIR"

echo "  Running HelloCone..."
"$BIN_DIR/02.1.HelloCone" 2>&1 | grep -i "success\|error"

echo "  Running EngineSpin..."
"$BIN_DIR/02.2.EngineSpin" 2>&1 | grep -i "success\|rendered\|error" | tail -1

echo "  Running Molecule..."
"$BIN_DIR/03.1.Molecule" 2>&1 | grep -i "success\|error" | tail -1

echo "  Running Robot..."
"$BIN_DIR/03.2.Robot" 2>&1 | grep -i "success\|error" | tail -1

# List generated files
echo ""
echo "Generated images:"
ls -lh *.rgb 2>/dev/null | awk '{print "  " $9 " - " $5}'

# Count files
NUM_FILES=$(ls -1 *.rgb 2>/dev/null | wc -l)
echo ""
echo "Total: $NUM_FILES images generated"

# Clean up Xvfb if we started it
if [ -n "$XVFB_PID" ]; then
    echo "Stopping Xvfb..."
    kill $XVFB_PID 2>/dev/null
fi
