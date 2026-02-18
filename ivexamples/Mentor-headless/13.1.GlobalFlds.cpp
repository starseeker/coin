/*
 * Headless version of Inventor Mentor example 13.1
 * 
 * Original: GlobalFlds - digital clock using realTime global field
 * Headless: Connects to realTime and renders at different time points
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/SbTime.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText3.h>
#include <cstdio>

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    SoSeparator *root = new SoSeparator;
    root->ref();

    // Add a camera, light, and material
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);
    
    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);
    root->addChild(myMaterial);

    // Create a Text3 object, and connect to the realTime field
    SoText3 *myText = new SoText3;
    root->addChild(myText);
    myText->string.connectFrom(SoDB::getGlobalField("realTime"));

    // Set up camera
    myCamera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    const char *baseFilename = (argc > 1) ? argv[1] : "13.1.GlobalFlds";
    char filename[256];

    // Render at current time
    printf("Current realTime value: %s\n", myText->string[0].getString());
    snprintf(filename, sizeof(filename), "%s_time1.rgb", baseFilename);
    renderToFile(root, filename);

    // Wait a bit and render again (simulate time passing)
    // We can't actually control realTime directly as it's maintained by the system
    // But we can demonstrate the connection is working
    printf("\nProcessing any pending updates...\n");
    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(TRUE);

    printf("Updated realTime value: %s\n", myText->string[0].getString());
    snprintf(filename, sizeof(filename), "%s_time2.rgb", baseFilename);
    renderToFile(root, filename);

    // Render once more
    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    
    printf("Final realTime value: %s\n", myText->string[0].getString());
    snprintf(filename, sizeof(filename), "%s_time3.rgb", baseFilename);
    renderToFile(root, filename);

    root->unref();
    return 0;
}
