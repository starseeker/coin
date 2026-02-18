/*
 * Headless version of Inventor Mentor example 13.7
 * 
 * Original: Rotor - rotating windmill vanes
 * Headless: Renders rotation sequence at different angles
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoRotor.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <cstdio>
#include <cmath>

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

    // Create a simple windmill tower (cylinder)
    SoMaterial *towerMat = new SoMaterial;
    towerMat->diffuseColor.setValue(0.5, 0.3, 0.1);
    root->addChild(towerMat);
    
    SoTransform *towerXform = new SoTransform;
    towerXform->translation.setValue(0, -2, 0);
    towerXform->scaleFactor.setValue(0.3, 3, 0.3);
    root->addChild(towerXform);
    root->addChild(new SoCylinder);

    // Add a rotor node to spin the vanes
    SoRotor *myRotor = new SoRotor;
    myRotor->rotation.setValue(SbVec3f(0, 0, 1), 0); // z axis
    myRotor->speed = 0.2;
    root->addChild(myRotor);

    // Create simple windmill vanes (4 thin cubes arranged as blades)
    SoSeparator *vanesSep = new SoSeparator;
    root->addChild(vanesSep);
    
    SoMaterial *vanesMat = new SoMaterial;
    vanesMat->diffuseColor.setValue(0.8, 0.8, 0.9);
    vanesSep->addChild(vanesMat);
    
    // Horizontal vane
    SoSeparator *vane1 = new SoSeparator;
    vanesSep->addChild(vane1);
    SoTransform *vane1Xform = new SoTransform;
    vane1Xform->scaleFactor.setValue(2, 0.1, 0.05);
    vane1->addChild(vane1Xform);
    vane1->addChild(new SoCube);
    
    // Vertical vane
    SoSeparator *vane2 = new SoSeparator;
    vanesSep->addChild(vane2);
    SoTransform *vane2Xform = new SoTransform;
    vane2Xform->scaleFactor.setValue(0.1, 2, 0.05);
    vane2->addChild(vane2Xform);
    vane2->addChild(new SoCube);

    // Setup camera to view the windmill
    camera->position.setValue(0, 0, 10);
    camera->pointAt(SbVec3f(0, 0, 0));

    const char *baseFilename = (argc > 1) ? argv[1] : "13.7.Rotor";
    char filename[256];

    // Enable the rotor
    myRotor->on = TRUE;

    // Render rotation sequence
    for (int i = 0; i <= 12; i++) {
        // Manually advance the rotor rotation
        float angle = i * M_PI / 6.0;  // 30 degrees per frame
        myRotor->rotation.setValue(SbVec3f(0, 0, 1), angle);
        
        // Process any pending updates
        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(TRUE);
        
        printf("Frame %d: Rotation angle = %.1f degrees\n", i, angle * 180.0 / M_PI);
        
        // Render this frame
        snprintf(filename, sizeof(filename), "%s_frame%02d.rgb", baseFilename, i);
        renderToFile(root, filename);
    }

    root->unref();
    return 0;
}
