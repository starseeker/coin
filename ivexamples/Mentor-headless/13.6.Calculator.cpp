/*
 * Headless version of Inventor Mentor example 13.6
 * 
 * Original: Calculator - uses calculator engine for complex motion paths
 * Headless: Demonstrates calculator engine with mathematical expressions
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoElapsedTime.h>
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

    // Create object with translation
    SoTranslation *objectTranslation = new SoTranslation;
    root->addChild(objectTranslation);
    
    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue(0.1, 0.8, 0.3);
    root->addChild(mat);
    root->addChild(new SoCube);

    // Set up elapsed time engine
    SoElapsedTime *timer = new SoElapsedTime;
    timer->ref();

    // Set up calculator engine for circular motion
    // x = cos(t), y = sin(t)
    SoCalculator *calc = new SoCalculator;
    calc->ref();
    calc->a.connectFrom(&timer->timeOut);
    calc->expression.set1Value(0, "oA = cos(a)");     // x coordinate
    calc->expression.set1Value(1, "oB = sin(a)");     // y coordinate
    calc->expression.set1Value(2, "oC = 0");          // z coordinate
    calc->expression.set1Value(3, "oD = vec3f(oA, oB, oC)");  // compose vector
    
    objectTranslation->translation.connectFrom(&calc->oD);

    const char *baseFilename = (argc > 1) ? argv[1] : "13.6.Calculator";
    char filename[256];

    // Render circular motion sequence
    for (int i = 0; i <= 16; i++) {
        float timeValue = i * M_PI / 8.0;  // 0 to 2*pi
        
        timer->timeIn.setValue(timeValue);
        
        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(TRUE);
        
        SbVec3f pos = objectTranslation->translation.getValue();
        printf("Time %.3f: Position = (%.2f, %.2f, %.2f)\n", 
               timeValue, pos[0], pos[1], pos[2]);
        
        snprintf(filename, sizeof(filename), "%s_frame%02d.rgb", baseFilename, i);
        renderToFile(root, filename);
    }

    timer->unref();
    calc->unref();
    root->unref();

    return 0;
}
