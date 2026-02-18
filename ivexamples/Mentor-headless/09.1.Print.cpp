/*
 * Headless version of Inventor Mentor example 9.1
 * 
 * Original: Print - renders scene to PostScript
 * Headless: Demonstrates offscreen rendering (already headless by nature)
 */

#include "headless_utils.h"
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <cstdio>

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    // Create a simple scene for demonstration
    SoSeparator *root = new SoSeparator;
    root->ref();

    // Add camera and light
    SoPerspectiveCamera *camera = new SoPerspectiveCamera;
    root->addChild(camera);
    root->addChild(new SoDirectionalLight);

    // Add some geometry
    SoMaterial *redMat = new SoMaterial;
    redMat->diffuseColor.setValue(1.0, 0.0, 0.0);
    root->addChild(redMat);
    
    SoTransform *leftTrans = new SoTransform;
    leftTrans->translation.setValue(-2, 0, 0);
    root->addChild(leftTrans);
    root->addChild(new SoCube);

    SoMaterial *blueMat = new SoMaterial;
    blueMat->diffuseColor.setValue(0.0, 0.5, 1.0);
    root->addChild(blueMat);
    
    SoTransform *rightTrans = new SoTransform;
    rightTrans->translation.setValue(4, 0, 0);
    root->addChild(rightTrans);
    root->addChild(new SoSphere);

    // Setup camera
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    const char *baseFilename = (argc > 1) ? argv[1] : "09.1.Print";
    char filename[256];

    // Render to RGB file (demonstrates offscreen rendering)
    snprintf(filename, sizeof(filename), "%s.rgb", baseFilename);
    renderToFile(root, filename);
    
    printf("Rendered scene using offscreen renderer\n");
    printf("Note: Original example printed to PostScript\n");

    root->unref();
    return 0;
}
