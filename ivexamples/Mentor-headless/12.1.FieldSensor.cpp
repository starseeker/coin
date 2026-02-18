/*
 * Headless version of Inventor Mentor example 12.1
 * 
 * Original: FieldSensor - monitors camera position changes
 * Headless: Programmatically changes camera position and captures sensor callbacks
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <cstdio>
#include <cmath>

// Store callback information for verification
int callbackCount = 0;

// Callback that reports whenever the camera's position changes
static void
cameraChangedCB(void *data, SoSensor *)
{
    SoCamera *camera = (SoCamera *)data;
    SbVec3f cameraPosition = camera->position.getValue();
    
    printf("Callback %d: Camera position: (%g, %g, %g)\n",
           ++callbackCount,
           cameraPosition[0], cameraPosition[1], cameraPosition[2]);
}

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    SoSeparator *root = new SoSeparator;
    root->ref();

    // Add camera and light
    SoPerspectiveCamera *camera = new SoPerspectiveCamera;
    root->addChild(camera);
    root->addChild(new SoDirectionalLight);
    
    // Add a simple cube to render
    root->addChild(new SoCube);

    // Set up the field sensor to monitor camera position
    SoFieldSensor *mySensor = new SoFieldSensor(cameraChangedCB, camera);
    mySensor->attach(&camera->position);

    // Set initial camera position
    camera->position.setValue(0, 0, 5);
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    const char *baseFilename = (argc > 1) ? argv[1] : "12.1.FieldSensor";
    char filename[256];

    // Render initial state
    snprintf(filename, sizeof(filename), "%s_initial.rgb", baseFilename);
    printf("\nRendering initial state...\n");
    renderToFile(root, filename);

    // Change camera position and process sensors
    printf("\nChanging camera position 1...\n");
    camera->position.setValue(2, 3, 10);
    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    snprintf(filename, sizeof(filename), "%s_pos1.rgb", baseFilename);
    renderToFile(root, filename);

    // Change camera position again
    printf("\nChanging camera position 2...\n");
    camera->position.setValue(-3, 2, 8);
    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    snprintf(filename, sizeof(filename), "%s_pos2.rgb", baseFilename);
    renderToFile(root, filename);

    // Change camera position once more
    printf("\nChanging camera position 3...\n");
    camera->position.setValue(0, -4, 6);
    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    snprintf(filename, sizeof(filename), "%s_pos3.rgb", baseFilename);
    renderToFile(root, filename);

    printf("\nTotal callbacks received: %d\n", callbackCount);

    delete mySensor;
    root->unref();
    return 0;
}
