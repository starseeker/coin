# Headless Mentor Examples

This directory contains headless, offscreen rendering versions of the Inventor Mentor examples. These examples have been converted from interactive GUI applications to standalone rendering programs that generate reference images for testing Coin's scene graph functionality.

## Overview

**Status**: 53 out of 53 convertible examples complete (100%)
**Coverage**: 53 out of 66 total examples (80%)

The remaining 13 examples are intrinsically toolkit-dependent (testing toolkit integration patterns rather than Coin core features).

## Purpose

The goal of these headless examples is to:
1. **Test toolkit-agnostic Coin features** - Validate that core Coin works without GUI frameworks
2. **Produce reference images** - Verify correct functioning during Coin development
3. **Enable automated testing** - Image comparison for regression testing
4. **Demonstrate architecture** - Prove the "toolkit as thin wrapper" model
5. **Establish integration patterns** - Show how to integrate Coin with any GUI toolkit

## Documentation

- **[STATUS.md](STATUS.md)** - Complete status of all 66 examples, organized by chapter
- **[NEW_CONVERSIONS.md](NEW_CONVERSIONS.md)** - Details of the 5 newest conversions (Chapters 14, 15, 17)
- **[TOOLKIT_AGNOSTIC_SUMMARY.md](TOOLKIT_AGNOSTIC_SUMMARY.md)** - Comprehensive analysis of what is/isn't toolkit-agnostic
- **[VALIDATION.md](VALIDATION.md)** - Validation against problem statement requirements
- **[MANIPULATOR_ANALYSIS.md](MANIPULATOR_ANALYSIS.md)** - Analysis proving manipulators are toolkit-agnostic
- **[CONVERSION_ANALYSIS.md](CONVERSION_ANALYSIS.md)** - Original conversion planning
- **[IMPLEMENTATION_NOTES.md](IMPLEMENTATION_NOTES.md)** - Technical implementation details
- **[TESTING_GUIDE.md](TESTING_GUIDE.md)** - Comprehensive guide to automated testing with image comparison
- **[CONTROL_IMAGE_GENERATION.md](CONTROL_IMAGE_GENERATION.md)** - How to generate control images for testing

## Building

### Prerequisites
- CMake 3.0 or later
- Coin library (built with offscreen rendering support)
- X11 libraries (or EGL for Linux)
- OpenGL libraries
- Xvfb (for headless execution on Linux)

### Build Instructions

```bash
cd ivexamples/Mentor-headless
mkdir build
cd build
cmake ..
make
```

### CMake Configuration Options

You can configure the image comparison sensitivity at build time:

```bash
cmake -DIMAGE_COMPARISON_HASH_THRESHOLD=10 \
      -DIMAGE_COMPARISON_RMSE_THRESHOLD=8.0 ..
```

**IMAGE_COMPARISON_HASH_THRESHOLD** (default: 5, range: 0-64):
- Controls perceptual hash comparison tolerance
- Lower = stricter, Higher = more tolerant
- Useful for handling platform-specific rendering differences

**IMAGE_COMPARISON_RMSE_THRESHOLD** (default: 5.0, range: 0-255):
- Controls RMSE (Root Mean Square Error) tolerance
- Lower = stricter, Higher = more tolerant
- Useful for handling anti-aliasing and minor color variations

## Automated Testing

The headless examples include a comprehensive testing framework for regression testing.

### Control Images

Control (reference) images are stored in the `control_images/` directory. These serve as the baseline for image comparison tests.

### Generating Control Images

To generate control images for all examples:

```bash
cd build
make generate_controls
```

Or manually:

```bash
cd ivexamples/Mentor-headless
BUILD_DIR=build ./generate_control_images.sh
```

### Running Tests

The project uses CTest for automated testing. Each test:
1. Runs the example to generate a test image
2. Compares it against the control image
3. Reports pass/fail based on configurable thresholds

Run all tests:

```bash
cd build
ctest
```

Run specific tests:

```bash
ctest -R "HelloCone"
ctest -R "Molecule"
```

Run with verbose output:

```bash
ctest -V
ctest -VV  # Extra verbose
```

### Image Comparison

The `image_comparator` utility provides three comparison methods:

1. **Pixel-perfect comparison** - Exact byte-for-byte match
2. **Perceptual hash comparison** - Approximate match using perceptual hashing algorithm
   - Handles minor rendering differences (anti-aliasing, OpenGL driver variations)
   - Configurable via `IMAGE_COMPARISON_HASH_THRESHOLD`
3. **RMSE comparison** - Root Mean Square Error between images
   - Handles color and brightness variations
   - Configurable via `IMAGE_COMPARISON_RMSE_THRESHOLD`

The comparator accepts approximate matches to handle:
- Platform-specific OpenGL rendering differences
- Font rendering variations across systems
- Anti-aliasing implementation differences
- Minor numerical precision differences in rendering

### Manual Image Comparison

You can manually compare images using the comparator:

```bash
./bin/image_comparator --threshold 10 --rmse 8.0 --verbose \
    control_images/02.1.HelloCone_control.rgb \
    test_output/02.1.HelloCone_test.rgb
```

Options:
- `--threshold N` - Set perceptual hash threshold (0-64)
- `--rmse N` - Set RMSE threshold (0-255)
- `--strict` - Require pixel-perfect match
- `--verbose` - Show detailed comparison metrics
- `--help` - Show usage information

## Running Examples

The examples require a display context to create OpenGL contexts, even for offscreen rendering. On Linux, you can use Xvfb (X Virtual Framebuffer):

```bash
# Start Xvfb in the background
Xvfb :99 -screen 0 1024x768x24 &
export DISPLAY=:99

# Run examples
cd build/output
COIN_GLX_PIXMAP_DIRECT_RENDERING=1 ../bin/02.1.HelloCone
COIN_GLX_PIXMAP_DIRECT_RENDERING=1 ../bin/02.2.EngineSpin
COIN_GLX_PIXMAP_DIRECT_RENDERING=1 ../bin/03.1.Molecule
```

### Output Format

Examples currently output SGI RGB format (.rgb files) which is supported natively by Coin without requiring external image libraries. These can be converted to other formats using ImageMagick:

```bash
# Convert to PNG
convert input.rgb output.png
```

## Framework

### headless_utils.h

This header provides utility functions for headless rendering:

- `initCoinHeadless()` - Initialize Coin for headless operation
- `renderToFile()` - Render a scene graph to an image file
- `findCamera()` - Find camera in scene graph
- `ensureCamera()` - Ensure scene has a camera (add if missing)
- `ensureLight()` - Ensure scene has a light (add if missing)
- `viewAll()` - Position camera to view entire scene
- `rotateCamera()` - Rotate camera around scene
- `simulateMousePress/Release/Motion/Drag()` - Mouse event simulation
- `simulateKeyPress/Release()` - Keyboard event simulation

## Converted Examples Summary

**53 examples** organized by chapter (see [STATUS.md](STATUS.md) for complete details):

### Core Features (Chapters 2-9): 37 examples
- Scene graphs, geometry, materials, cameras, lights
- Textures, NURBS curves and surfaces
- Actions (print, search, pick, callback)

### Dynamic Features (Chapters 12-13): 12 examples
- Sensors (field, node, alarm, timer)
- Engines (time, calculator, gate, boolean, rotor, blinker)

### Advanced Features (Chapters 14-15, 17): 9 examples
- **NEW: Chapter 14** - NodeKits with time-based animation and keyboard interaction
- **NEW: Chapter 15** - Manipulators/draggers with programmatic control
- **NEW: Chapter 17** - OpenGL callback integration

### Selected Example Highlights

#### Chapter 2: Basic Examples
- **02.1.HelloCone** - Simple red cone rendering
- **02.2.EngineSpin** - Animated spinning cone (renders multiple frames)
- **02.3.Trackball** - Trackball rotation simulation
- **02.4.Examiner** - Examiner viewer with predefined viewpoints

#### Chapter 14: NodeKits (NEW)
- **14.1.FrolickingWords** - Animated 3D text using engines and nodekits
- **14.3.Balance** - Balance scale with keyboard-driven motion hierarchy

#### Chapter 15: Manipulators (NEW - Complete)
- **15.1.ConeRadius** - Dragger controlling cone radius via engine
- **15.2.SliderBox** - Three translate1Draggers with programmatic control
- **15.3.AttachManip** - Attaching/detaching different manipulator types
- **15.4.Customize** - Custom dragger geometry via nodekit parts

#### Chapter 17: OpenGL Integration (NEW)
- **17.2.GLCallback** - Custom OpenGL rendering through callback node

See [STATUS.md](STATUS.md) for the complete list of all 53 examples.

## What Was NOT Converted (13 examples)

Examples that are intrinsically toolkit-dependent (testing toolkit integration, not Coin features):

- **Chapter 10** (3 examples): Xt render area callbacks, Motif widgets
- **Chapter 14** (1 example): Widget-based material/transform editors
- **Chapter 16** (5 examples): ExaminerViewer customization, overlay planes, window management
- **Chapter 17** (2 examples): GLX context creation, X11 color management

These examples test **how to integrate Coin with toolkits**, not Coin's core features.

## Architecture Validation

These conversions validate the **"toolkit as thin wrapper"** architecture:

### What Coin Core Provides (Toolkit-Agnostic)
✅ Complete scene graph management
✅ Rendering to any OpenGL context (or offscreen)
✅ Event abstraction and processing (SoEvent system)
✅ Manipulators/draggers (no toolkit dependencies)
✅ Engines and time-based animations
✅ OpenGL integration via callbacks
✅ NodeKits and hierarchical organization

### What Toolkits Must Provide (Minimal Interface)
- Window with OpenGL context
- Mouse position (X, Y coordinates)
- Mouse button states (press/release)
- Keyboard key events
- Viewport dimensions
- Display refresh trigger

### Integration Pattern
```cpp
// Toolkit captures native event
toolkit->onMouseEvent([&](NativeEvent* evt) {
    // Translate to Coin event
    SoMouseButtonEvent coinEvent;
    coinEvent.setPosition(SbVec2s(evt->x(), evt->y()));
    
    // Apply to scene
    SoHandleEventAction action(viewport);
    action.setEvent(&coinEvent);
    action.apply(sceneRoot);
    
    // Redraw if handled
    if (action.isHandled()) toolkit->scheduleRedraw();
});
```

This pattern works with **any** GUI toolkit (Qt, FLTK, custom, etc.).

## Interaction Simulation

Examples that originally required user interaction have been converted to generate predetermined sequences:

- **Rotation/viewing**: Multiple frames from different camera angles
- **Animation**: Time-based sequences rendered at fixed intervals
- **Selection/picking**: Programmatically select objects and render states
- **Keyboard/mouse**: Event simulation through SoHandleEventAction

## Technical Notes

### OpenGL Context

Coin's offscreen renderer requires an OpenGL context even for headless rendering. On Linux, this is provided through:
- GLX (with Xvfb providing the X server)
- Direct rendering may need to be enabled: `COIN_GLX_PIXMAP_DIRECT_RENDERING=1`

### Image Formats

The native writeToRGB() method is used as it doesn't require external dependencies. For other formats (PNG, JPEG), you would need:
- simage library installed and linked
- Or post-process RGB files with conversion tools

### Performance

Offscreen rendering performance depends on:
- Available OpenGL implementation (hardware vs software)
- Image resolution
- Scene complexity
- Number of frames/viewpoints

## Future Work

Additional examples to be converted:
- Chapter 4: Cameras and Lights
- Chapter 5-7: Geometry, Materials, and Textures
- Chapter 8-9: NURBS and Rendering Actions
- Chapter 12-13: Sensors and Engines
- Chapter 15: Remaining manipulators (15.2, 15.4)

## Manipulators

Manipulator examples (Chapter 15) demonstrate that manipulators are **completely toolkit-agnostic** and work with Coin's internal event system. See `MANIPULATOR_ANALYSIS.md` for detailed analysis proving manipulators are self-contained within Coin and suitable for headless testing.

The event simulation infrastructure in `headless_utils.h` provides:
- Mouse button press/release simulation
- Mouse drag gesture simulation  
- Keyboard event simulation
- Proper viewport context for coordinate transformation

This establishes the pattern for future toolkit integration where GUI frameworks translate their native events into Coin's SoEvent objects.

## Contributing

When converting new examples:
1. Remove all GUI-specific code (So@Gui@, render areas, viewers)
2. Ensure scene has camera and light
3. Use renderToFile() for output
4. For animations, render multiple frames with explicit time/angle control
5. For interactions, simulate procedurally with fixed sequences
6. Document any special setup in comments
