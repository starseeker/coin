/*
 * Headless version of Inventor Mentor example 2.4
 * 
 * Original: Examiner - uses an examiner viewer to look at a cone
 * Headless: Simulates examiner viewer operations (tumble, dolly, pan)
 */

#include "headless_utils.h"
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <cmath>
#include <cstdio>

int main(int argc, char **argv)
{
    initCoinHeadless();

    SoSeparator *root = new SoSeparator;
    root->ref();

    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    root->addChild(myCamera);
    root->addChild(new SoDirectionalLight);

    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);  // Red cone
    root->addChild(myMaterial);
    root->addChild(new SoCone);

    // Setup camera to view the scene
    SbViewportRegion viewport(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    myCamera->viewAll(root, viewport);

    const char *baseFilename = (argc > 1) ? argv[1] : "02.4.Examiner";
    char filename[256];

    int frameNum = 0;

    // Initial view
    snprintf(filename, sizeof(filename), "%s_frame%02d_initial.rgb", baseFilename, frameNum++);
    renderToFile(root, filename);

    // Simulate tumbling (rotating camera around scene)
    SbVec3f origPos = myCamera->position.getValue();
    SbRotation origOrient = myCamera->orientation.getValue();
    
    for (int i = 1; i <= 8; i++) {
        float angle = (M_PI / 4.0f) * i;  // 45 degree increments
        
        // Rotate around Y axis
        float radius = origPos.length();
        myCamera->position.setValue(
            radius * sin(angle),
            origPos[1],
            radius * cos(angle)
        );
        myCamera->pointAt(SbVec3f(0, 0, 0));
        
        snprintf(filename, sizeof(filename), "%s_frame%02d_tumble.rgb", baseFilename, frameNum++);
        renderToFile(root, filename);
    }

    // Reset position for dolly operations
    myCamera->position.setValue(origPos);
    myCamera->orientation.setValue(origOrient);

    // Simulate dollying (zooming in/out by moving camera)
    for (int i = 0; i < 4; i++) {
        float scale = 0.5f + i * 0.5f;  // 0.5, 1.0, 1.5, 2.0
        SbVec3f scaledPos = origPos * scale;
        myCamera->position.setValue(scaledPos);
        
        snprintf(filename, sizeof(filename), "%s_frame%02d_dolly.rgb", baseFilename, frameNum++);
        renderToFile(root, filename);
    }

    printf("Rendered %d frames simulating examiner viewer operations\n", frameNum);

    root->unref();
    return 0;
}
