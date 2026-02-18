/*
 * Headless version of Inventor Mentor example 13.4
 * 
 * Original: Gate - toggles gate to enable/disable motion
 * Headless: Demonstrates gate engine on/off states
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/engines/SoGate.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <cstdio>
#include <cmath>

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    SoSeparator *root = new SoSeparator;
    root->ref();

    // Add a camera and light
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    myCamera->position.setValue(0., 0., 10.0);
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);

    // Create moving object
    SoTranslation *objectTranslation = new SoTranslation;
    root->addChild(objectTranslation);
    
    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue(0.8, 0.3, 0.1);
    root->addChild(mat);
    root->addChild(new SoCube);

    // Set up elapsed time engine
    SoElapsedTime *myCounter = new SoElapsedTime;
    myCounter->ref();

    // Set up gate engine to control whether time passes through
    SoGate *myGate = new SoGate(SoMFFloat::getClassTypeId());
    myGate->ref();
    myGate->input->connectFrom(&myCounter->timeOut);

    // Connect gate output to translation
    SoComposeVec3f *compose = new SoComposeVec3f;
    compose->ref();
    compose->x.connectFrom(myGate->output);
    objectTranslation->translation.connectFrom(&compose->vector);

    const char *baseFilename = (argc > 1) ? argv[1] : "13.4.Gate";
    char filename[256];

    // Render with gate disabled (object should not move)
    printf("=== Gate DISABLED ===\n");
    myGate->enable = FALSE;
    
    for (int i = 0; i < 5; i++) {
        float timeValue = i * 0.5f;
        myCounter->timeIn.setValue(timeValue);
        
        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(TRUE);
        
        SbVec3f pos = objectTranslation->translation.getValue();
        printf("Time %.1f: Position = %.2f (gate disabled)\n", timeValue, pos[0]);
        
        snprintf(filename, sizeof(filename), "%s_disabled_%02d.rgb", baseFilename, i);
        renderToFile(root, filename);
    }

    // Render with gate enabled (object should move)
    printf("\n=== Gate ENABLED ===\n");
    myGate->enable = TRUE;
    
    for (int i = 0; i < 5; i++) {
        float timeValue = i * 0.5f;
        myCounter->timeIn.setValue(timeValue);
        
        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(TRUE);
        
        SbVec3f pos = objectTranslation->translation.getValue();
        printf("Time %.1f: Position = %.2f (gate enabled)\n", timeValue, pos[0]);
        
        snprintf(filename, sizeof(filename), "%s_enabled_%02d.rgb", baseFilename, i);
        renderToFile(root, filename);
    }

    myCounter->unref();
    myGate->unref();
    compose->unref();
    root->unref();

    return 0;
}
