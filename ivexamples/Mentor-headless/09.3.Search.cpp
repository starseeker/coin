/*
 * Headless version of Inventor Mentor example 9.3
 * 
 * Original: Search - uses search action to find lights
 * Headless: Demonstrates search action and renders result
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <cstdio>

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    // Create a scene without a light
    SoSeparator *root = new SoSeparator;
    root->ref();

    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(0.8, 0.3, 0.1);
    root->addChild(myMaterial);
    root->addChild(new SoCube);

    // Use search action to look for lights
    SoSearchAction mySearchAction;
    mySearchAction.setType(SoLight::getClassTypeId());
    mySearchAction.setInterest(SoSearchAction::FIRST);
    mySearchAction.apply(root);
    
    const char *baseFilename = (argc > 1) ? argv[1] : "09.3.Search";
    char filename[256];

    // Render before adding light
    SoPerspectiveCamera *camera = new SoPerspectiveCamera;
    root->insertChild(camera, 0);
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));
    
    snprintf(filename, sizeof(filename), "%s_no_light.rgb", baseFilename);
    renderToFile(root, filename);
    printf("Rendered scene without light\n");

    // Check search result and add light if none found
    if (mySearchAction.getPath() == NULL) {
        printf("Search Action: No lights found - adding default light\n");
        SoDirectionalLight *myLight = new SoDirectionalLight;
        root->insertChild(myLight, 1); // After camera
    } else {
        printf("Search Action: Light already exists\n");
    }

    // Render after ensuring light exists
    snprintf(filename, sizeof(filename), "%s_with_light.rgb", baseFilename);
    renderToFile(root, filename);
    printf("Rendered scene with light\n");

    root->unref();
    return 0;
}
