/*
 * Headless version of Inventor Mentor example 12.2
 * 
 * Original: NodeSensor - monitors node changes using getTriggerNode/Field
 * Headless: Programmatically modifies nodes and renders states
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <cstdio>

// Sensor callback function
static void
rootChangedCB(void *, SoSensor *s)
{
    // We know the sensor is really a data sensor
    SoDataSensor *mySensor = (SoDataSensor *)s;
    
    SoNode *changedNode = mySensor->getTriggerNode();
    SoField *changedField = mySensor->getTriggerField();
    
    printf("The node named '%s' changed",
           changedNode->getName().getString());

    if (changedField != NULL) {
        SbName fieldName;
        changedNode->getFieldName(changedField, fieldName);
        printf(" (field %s)\n", fieldName.getString());
    } else {
        printf(" (no fields changed)\n");
    }
}

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    SoSeparator *root = new SoSeparator;
    root->ref();
    root->setName("Root");

    // Add camera and light for rendering
    SoPerspectiveCamera *camera = new SoPerspectiveCamera;
    root->addChild(camera);
    root->addChild(new SoDirectionalLight);

    // Add shapes
    SoCube *myCube = new SoCube;
    root->addChild(myCube);
    myCube->setName("MyCube");

    SoSphere *mySphere = new SoSphere;
    root->addChild(mySphere);
    mySphere->setName("MySphere");

    // Set up camera
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    // Create and attach node sensor
    SoNodeSensor *mySensor = new SoNodeSensor;
    mySensor->setPriority(0);
    mySensor->setFunction(rootChangedCB);
    mySensor->attach(root);

    const char *baseFilename = (argc > 1) ? argv[1] : "12.2.NodeSensor";
    char filename[256];

    // Render initial state
    printf("\n=== Initial state ===\n");
    snprintf(filename, sizeof(filename), "%s_initial.rgb", baseFilename);
    renderToFile(root, filename);

    // Change cube width
    printf("\n=== Changing cube width ===\n");
    myCube->width = 3.0;
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    snprintf(filename, sizeof(filename), "%s_cube_width.rgb", baseFilename);
    renderToFile(root, filename);

    // Change cube height
    printf("\n=== Changing cube height ===\n");
    myCube->height = 4.0;
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    snprintf(filename, sizeof(filename), "%s_cube_height.rgb", baseFilename);
    renderToFile(root, filename);

    // Change sphere radius
    printf("\n=== Changing sphere radius ===\n");
    mySphere->radius = 2.0;
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    snprintf(filename, sizeof(filename), "%s_sphere_radius.rgb", baseFilename);
    renderToFile(root, filename);

    // Remove sphere
    printf("\n=== Removing sphere ===\n");
    root->removeChild(mySphere);
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    snprintf(filename, sizeof(filename), "%s_removed_sphere.rgb", baseFilename);
    renderToFile(root, filename);

    delete mySensor;
    root->unref();

    return 0;
}
