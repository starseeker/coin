# Headless Mentor Examples

This directory contains headless, offscreen rendering versions of the Inventor Mentor examples. These examples have been converted from interactive GUI applications to standalone rendering programs that generate reference images for testing Coin's scene graph functionality.

## Purpose

The goal of these headless examples is to:
1. Produce reference images for verifying correct functioning during the large-scale rework of Coin
2. Provide a test suite that exercises core scene graph elements without requiring GUI frameworks
3. Enable automated testing through image comparison
4. Demonstrate headless/offscreen rendering capabilities of Coin

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

## Converted Examples

### Chapter 2: Basic Examples
- **02.1.HelloCone** - Simple red cone rendering
- **02.2.EngineSpin** - Animated spinning cone (renders multiple frames)
- **02.3.Trackball** - Trackball rotation simulation
- **02.4.Examiner** - Examiner viewer with predefined viewpoints

### Chapter 3: Scene Graphs  
- **03.1.Molecule** - Water molecule from multiple viewpoints
- **03.2.Robot** - Robot assembly with shared geometry
- **03.3.Naming** - Named nodes demonstration

(Chapters 4-14 examples omitted for brevity - see CONVERSION_ANALYSIS.md)

### Chapter 15: Manipulators
- **15.1.ConeRadius** - Dragger controlling cone radius via engine
- **15.3.AttachManip** - Attaching/detaching different manipulator types

## Examples Skipped

The following types of examples are not converted as they rely on features being removed in the Coin rework:

- VRML97-specific examples
- XML-based examples (X3D, etc.)
- Examples requiring specific GUI toolkit widgets (Motif/Xt)

## Interaction Simulation

Examples that originally required user interaction have been converted to generate predetermined sequences:

- **Rotation/viewing**: Multiple frames from different camera angles
- **Animation**: Time-based sequences rendered at fixed intervals
- **Selection/picking**: Programmatically select objects and render states

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
