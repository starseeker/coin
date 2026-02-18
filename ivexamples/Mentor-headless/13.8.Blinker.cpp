/*
 * Headless version of Inventor Mentor example 13.8
 * 
 * Original: Blinker - blinking neon sign with fast and slow blinkers
 * Headless: Renders blink sequence showing on/off states
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoBlinker.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <cstdio>
#include <cmath>

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    SoSeparator *root = new SoSeparator;
    root->ref();

    // Set up camera and light
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);

    // Add the non-blinking part (static text)
    SoMaterial *staticMat = new SoMaterial;
    staticMat->diffuseColor.setValue(0.8, 0.8, 0.8);
    root->addChild(staticMat);
    
    SoTransform *staticXform = new SoTransform;
    staticXform->translation.setValue(0, 2, 0);
    root->addChild(staticXform);
    
    SoText3 *staticText = new SoText3;
    staticText->string = "EAT AT";
    root->addChild(staticText);

    // Add the fast-blinking part to a blinker node
    SoBlinker *fastBlinker = new SoBlinker;
    root->addChild(fastBlinker);
    fastBlinker->speed = 2;  // 2 Hz: 2 complete on/off cycles per second
    
    SoSeparator *fastSep = new SoSeparator;
    fastBlinker->addChild(fastSep);
    
    SoMaterial *fastMat = new SoMaterial;
    fastMat->diffuseColor.setValue(1.0, 0.0, 0.0);
    fastSep->addChild(fastMat);
    
    SoText3 *fastText = new SoText3;
    fastText->string = "JOSIE'S";
    fastSep->addChild(fastText);

    // Add the slow-blinking part to another blinker node
    SoBlinker *slowBlinker = new SoBlinker;
    root->addChild(slowBlinker);
    slowBlinker->speed = 0.5;  // 0.5 Hz: 2 seconds per complete on/off cycle
    
    SoSeparator *slowSep = new SoSeparator;
    slowBlinker->addChild(slowSep);
    
    SoMaterial *slowMat = new SoMaterial;
    slowMat->diffuseColor.setValue(0.0, 1.0, 0.0);
    slowSep->addChild(slowMat);
    
    SoTransform *slowXform = new SoTransform;
    slowXform->translation.setValue(0, -2, 0);
    slowSep->addChild(slowXform);
    
    SoText3 *slowText = new SoText3;
    slowText->string = "OPEN";
    slowSep->addChild(slowText);

    // Setup camera
    myCamera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    const char *baseFilename = (argc > 1) ? argv[1] : "13.8.Blinker";
    char filename[256];

    // Render blink sequence
    // Fast blinker cycles at 2 Hz (0.5 sec period)
    // Slow blinker cycles at 0.5 Hz (2 sec period)
    for (int i = 0; i <= 16; i++) {
        float time = i * 0.25f;  // 0, 0.25, 0.5, ... 4.0 seconds
        
        // Manually set the blinker states based on time
        // Fast blinker: on/off every 0.25 seconds
        fastBlinker->whichChild = (int)(time * 2) % 2;
        
        // Slow blinker: on/off every 1 second
        slowBlinker->whichChild = (int)(time * 0.5) % 2;
        
        printf("Time %.2f: Fast=%s, Slow=%s\n", 
               time,
               fastBlinker->whichChild.getValue() == 0 ? "ON " : "OFF",
               slowBlinker->whichChild.getValue() == 0 ? "ON " : "OFF");
        
        // Process updates
        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(TRUE);
        
        // Render this frame
        snprintf(filename, sizeof(filename), "%s_frame%02d.rgb", baseFilename, i);
        renderToFile(root, filename);
    }

    root->unref();
    return 0;
}
