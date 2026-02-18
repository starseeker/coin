/*
 * Headless version of Inventor Mentor example 2.3
 * 
 * Original: Trackball - uses a trackball manipulator to rotate a cone
 * Headless: Simulates orbital camera movement around the cone
 */

#include "headless_utils.h"
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <cmath>
#include <cstdio>

int main(int argc, char **argv)
{
    initCoinHeadless();

    SoSeparator *root = new SoSeparator;
    root->ref();

    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);

    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);  // Red cone
    root->addChild(myMaterial);
    root->addChild(new SoCone);

    // Setup camera to view the scene
    SbViewportRegion viewport(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    myCamera->viewAll(root, viewport);

    const char *baseFilename = (argc > 1) ? argv[1] : "02.3.Trackball";
    char filename[256];

    // Simulate trackball rotation by orbiting camera around the cone
    // This demonstrates what a trackball manipulator would do
    const int numFrames = 16;
    const float radius = 5.0f;
    const float height = 3.0f;

    for (int i = 0; i < numFrames; i++) {
        float angle = (2.0f * M_PI * i) / numFrames;
        
        // Position camera in an orbit
        myCamera->position.setValue(
            radius * cos(angle),
            height,
            radius * sin(angle)
        );
        
        // Point camera at center of scene
        myCamera->pointAt(SbVec3f(0, 0, 0));
        
        snprintf(filename, sizeof(filename), "%s_frame%02d.rgb", baseFilename, i);
        renderToFile(root, filename);
    }

    printf("Rendered %d frames simulating trackball rotation\n", numFrames);

    root->unref();
    return 0;
}
