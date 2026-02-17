/*
 * Utility functions for headless rendering of Coin examples
 * 
 * This provides common functionality for converting interactive
 * Mentor examples to headless, offscreen rendering tests that
 * produce reference images for validation.
 */

#ifndef HEADLESS_UTILS_H
#define HEADLESS_UTILS_H

#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <cstdio>
#include <cstring>

// Default image dimensions
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

/**
 * Initialize Coin database for headless operation
 */
inline void initCoinHeadless() {
    SoDB::init();
}

/**
 * Render a scene to an image file
 * @param root Scene graph root
 * @param filename Output filename (extension determines format: .png, .jpg, .rgb)
 * @param width Image width
 * @param height Image height
 * @param backgroundColor Background color (default: black)
 * @return true if successful
 */
inline bool renderToFile(
    SoNode *root, 
    const char *filename,
    int width = DEFAULT_WIDTH,
    int height = DEFAULT_HEIGHT,
    const SbColor &backgroundColor = SbColor(0.0f, 0.0f, 0.0f))
{
    if (!root || !filename) {
        fprintf(stderr, "Error: Invalid parameters to renderToFile\n");
        return false;
    }

    // Create viewport and renderer
    SbViewportRegion viewport(width, height);
    SoOffscreenRenderer renderer(viewport);
    renderer.setComponents(SoOffscreenRenderer::RGB);
    renderer.setBackgroundColor(backgroundColor);

    // Render the scene
    if (!renderer.render(root)) {
        fprintf(stderr, "Error: Failed to render scene\n");
        return false;
    }

    // Write to file
    if (!renderer.writeToRGB(filename)) {
        fprintf(stderr, "Error: Failed to write to RGB file %s\n", filename);
        // Try writeToFile as fallback
        const char *ext = strrchr(filename, '.');
        if (ext) ext++; // Skip the dot
        else ext = "rgb"; // Default to RGB
        
        if (!renderer.writeToFile(filename, ext)) {
            fprintf(stderr, "Error: Failed to write to file %s\n", filename);
            return false;
        }
    }

    printf("Successfully rendered to %s (%dx%d)\n", filename, width, height);
    return true;
}

/**
 * Find camera in scene graph
 * @param root Scene graph root
 * @return First camera found, or NULL
 */
inline SoCamera* findCamera(SoNode *root) {
    SoSearchAction search;
    search.setType(SoCamera::getClassTypeId());
    search.setInterest(SoSearchAction::FIRST);
    search.apply(root);
    
    if (search.getPath()) {
        return (SoCamera*)search.getPath()->getTail();
    }
    return NULL;
}

/**
 * Ensure scene has a camera, add one if missing
 * @param root Scene graph separator (must be SoSeparator or compatible)
 * @return Camera in scene (existing or newly added)
 */
inline SoCamera* ensureCamera(SoSeparator *root) {
    SoCamera *camera = findCamera(root);
    if (camera) {
        return camera;
    }
    
    // Add a default perspective camera
    SoPerspectiveCamera *newCam = new SoPerspectiveCamera;
    root->insertChild(newCam, 0);
    return newCam;
}

/**
 * Ensure scene has a light, add one if missing
 * @param root Scene graph separator
 */
inline void ensureLight(SoSeparator *root) {
    SoSearchAction search;
    search.setType(SoDirectionalLight::getClassTypeId());
    search.setInterest(SoSearchAction::FIRST);
    search.apply(root);
    
    if (!search.getPath()) {
        // Add a default directional light
        SoDirectionalLight *light = new SoDirectionalLight;
        // Insert after camera (if exists) or at beginning
        SoCamera *cam = findCamera(root);
        int insertPos = 0;
        if (cam) {
            for (int i = 0; i < root->getNumChildren(); i++) {
                if (root->getChild(i) == cam) {
                    insertPos = i + 1;
                    break;
                }
            }
        }
        root->insertChild(light, insertPos);
    }
}

/**
 * Setup camera to view entire scene
 * @param root Scene graph root
 * @param camera Camera to position
 * @param viewport Viewport region for aspect ratio
 */
inline void viewAll(SoNode *root, SoCamera *camera, const SbViewportRegion &viewport) {
    if (!camera) return;
    camera->viewAll(root, viewport);
}

/**
 * Rotate camera around scene by specified angles
 * @param camera Camera to rotate
 * @param azimuth Horizontal rotation in radians
 * @param elevation Vertical rotation in radians
 */
inline void rotateCamera(SoCamera *camera, float azimuth, float elevation) {
    if (!camera) return;
    
    // Get current position and orientation
    SbVec3f position = camera->position.getValue();
    SbRotation orientation = camera->orientation.getValue();
    
    // Create rotation around Y axis (azimuth) and X axis (elevation)
    SbRotation azimuthRot(SbVec3f(0, 1, 0), azimuth);
    SbRotation elevationRot(SbVec3f(1, 0, 0), elevation);
    
    // Apply rotations
    SbRotation newOrientation = orientation * azimuthRot * elevationRot;
    camera->orientation.setValue(newOrientation);
}

/**
 * Create a simple test scene if scene creation fails
 * @return A simple scene with a cone
 */
SoSeparator* createFallbackScene();

#endif // HEADLESS_UTILS_H
