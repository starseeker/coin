/*
 * Headless version of Inventor Mentor example 15.1
 * 
 * Original: ConeRadius - Uses SoTranslate1Dragger to control cone bottomRadius
 * Headless: Demonstrates dragger control of geometry via engine connections
 * 
 * The dragger's translation field connects to an SoDecomposeVec3f engine,
 * which extracts the x component and feeds it to the cone's bottomRadius.
 * In headless mode, we programmatically set the dragger position and render
 * the results to show the cone responding to the dragger's value.
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/draggers/SoTranslate1Dragger.h>
#include <cstdio>

int main(int argc, char **argv)
{
    initCoinHeadless();

    SoSeparator *root = new SoSeparator;
    root->ref();

    // Add camera and light
    SoPerspectiveCamera *camera = new SoPerspectiveCamera;
    root->addChild(camera);
    root->addChild(new SoDirectionalLight);

    // Create myDragger with an initial translation of (1,0,0)
    SoTranslate1Dragger *myDragger = new SoTranslate1Dragger;
    root->addChild(myDragger);
    myDragger->translation.setValue(1, 0, 0);

    // Place an SoCone above myDragger
    SoTransform *myTransform = new SoTransform;
    SoCone *myCone = new SoCone;
    root->addChild(myTransform);
    root->addChild(myCone);
    myTransform->translation.setValue(0, 3, 0);

    // SoDecomposeVec3f engine extracts myDragger's x-component
    // The result is connected to myCone's bottomRadius.
    SoDecomposeVec3f *myEngine = new SoDecomposeVec3f;
    myEngine->ref();
    myEngine->vector.connectFrom(&myDragger->translation);
    myCone->bottomRadius.connectFrom(&myEngine->x);

    // Setup camera to view the scene
    SbViewportRegion viewport(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    camera->viewAll(root, viewport);

    const char *baseFilename = (argc > 1) ? argv[1] : "15.1.ConeRadius";
    char filename[256];

    int frameNum = 0;

    // Test different dragger positions to show cone radius changing
    float testPositions[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f};
    int numPositions = sizeof(testPositions) / sizeof(testPositions[0]);

    printf("=== Dragger Controls Cone Radius via Engine ===\n");
    printf("Demonstrating SoTranslate1Dragger connected to cone bottomRadius\n\n");

    for (int i = 0; i < numPositions; i++) {
        float xPos = testPositions[i];
        
        // Set dragger position programmatically (simulates user dragging)
        myDragger->translation.setValue(xPos, 0, 0);
        
        printf("Frame %d: Dragger X = %.1f, Cone bottomRadius = %.1f\n",
               frameNum, xPos, myCone->bottomRadius.getValue());
        
        snprintf(filename, sizeof(filename), "%s_frame%02d_radius%.1f.rgb",
                 baseFilename, frameNum, xPos);
        renderToFile(root, filename);
        frameNum++;
    }

    printf("\n=== Demonstrating Engine Connection ===\n");
    printf("The dragger's translation.x automatically updates cone->bottomRadius\n");
    printf("This shows how draggers can control scene parameters via engines.\n");
    printf("In interactive mode, the user would drag the manipulator handle.\n");
    printf("Rendered %d frames showing different cone radii\n", frameNum);

    myEngine->unref();
    root->unref();

    return 0;
}
