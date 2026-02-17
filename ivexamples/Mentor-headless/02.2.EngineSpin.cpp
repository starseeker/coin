/*
 * Headless version of Inventor Mentor example 2.2
 * 
 * Original: EngineSpin - uses an engine to make a cone spin
 * Headless: Renders multiple frames showing the cone at different rotation angles
 */

#include "headless_utils.h"
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSeparator.h>
#include <cmath>
#include <cstdio>

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    SoSeparator *root = new SoSeparator;
    root->ref();
    
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);

    // This transformation is used to rotate the cone
    SoRotationXYZ *myRotXYZ = new SoRotationXYZ;
    myRotXYZ->axis = SoRotationXYZ::X;     // rotate about X axis
    root->addChild(myRotXYZ);

    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);   // Red
    root->addChild(myMaterial);
    root->addChild(new SoCone);

    // Setup camera
    myCamera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    // Render multiple frames at different rotation angles
    // simulating the spinning animation
    const char *baseFilename = (argc > 1) ? argv[1] : "02.2.EngineSpin";
    char filename[256];
    
    int numFrames = 8;  // Number of frames to render
    for (int i = 0; i < numFrames; i++) {
        float angle = (2.0f * M_PI * i) / numFrames;
        myRotXYZ->angle.setValue(angle);
        
        snprintf(filename, sizeof(filename), "%s_frame%02d.png", baseFilename, i);
        renderToFile(root, filename);
    }
    
    printf("Rendered %d frames showing rotation animation\n", numFrames);

    root->unref();
    return 0;
}
