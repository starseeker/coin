/*
 * Headless version of Inventor Mentor example 2.1
 * 
 * Original: Hello Cone - draws a red cone in a window
 * Headless: Renders a red cone to an image file
 */

#include "headless_utils.h"
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    // Make a scene containing a red cone
    SoSeparator *root = new SoSeparator;
    root->ref();
    
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);
    
    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);   // Red
    root->addChild(myMaterial);
    root->addChild(new SoCone);

    // Make camera see everything
    myCamera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    // Render to file
    const char *filename = (argc > 1) ? argv[1] : "02.1.HelloCone.png";
    if (!renderToFile(root, filename)) {
        root->unref();
        return 1;
    }

    root->unref();
    return 0;
}
