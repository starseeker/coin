/*
 * Headless version of Inventor Mentor example 10.1
 * 
 * Original: addEventCB - demonstrates keyboard event callbacks for interactive scaling
 * Headless: Simulates keyboard events to scale objects up and down
 * 
 * This example demonstrates the event simulation pattern developed for manipulators:
 * - Uses simulateKeyPress/Release from headless_utils.h
 * - Proper event callback registration and handling
 * - Events trigger callbacks just like in interactive mode
 */

#include "headless_utils.h"
#include <Inventor/Sb.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <cstdio>

// Global transforms for each object (from original example)
SoTransform *cubeTransform, *sphereTransform, *coneTransform, *cylTransform;

// Event callback function - called when UP_ARROW or DOWN_ARROW is pressed
// This matches the pattern from the original example
void myKeyPressCB(void *userData, SoEventCallback *eventCB)
{
    SoSelection *selection = (SoSelection *) userData;
    const SoEvent *event = eventCB->getEvent();

    // Check for the Up and Down arrow keys being pressed
    if (SO_KEY_PRESS_EVENT(event, UP_ARROW)) {
        printf("UP_ARROW detected - scaling up\n");
        // Scale up selected objects
        for (int i = 0; i < selection->getNumSelected(); i++) {
            SoPath *selectedPath = selection->getPath(i);
            SoTransform *xform = NULL;

            // Look for the shape node to identify which transform to modify
            for (int j = 0; j < selectedPath->getLength() && (xform == NULL); j++) {
                SoNode *n = selectedPath->getNodeFromTail(j);
                if (n->isOfType(SoCube::getClassTypeId())) {
                    xform = cubeTransform;
                } else if (n->isOfType(SoCone::getClassTypeId())) {
                    xform = coneTransform;
                } else if (n->isOfType(SoSphere::getClassTypeId())) {
                    xform = sphereTransform;
                } else if (n->isOfType(SoCylinder::getClassTypeId())) {
                    xform = cylTransform;
                }
            }

            if (xform) {
                SbVec3f currentScale = xform->scaleFactor.getValue();
                currentScale *= 1.1f;
                xform->scaleFactor.setValue(currentScale);
            }
        }
        eventCB->setHandled();
    } else if (SO_KEY_PRESS_EVENT(event, DOWN_ARROW)) {
        printf("DOWN_ARROW detected - scaling down\n");
        // Scale down selected objects
        for (int i = 0; i < selection->getNumSelected(); i++) {
            SoPath *selectedPath = selection->getPath(i);
            SoTransform *xform = NULL;

            // Look for the shape node to identify which transform to modify
            for (int j = 0; j < selectedPath->getLength() && (xform == NULL); j++) {
                SoNode *n = selectedPath->getNodeFromTail(j);
                if (n->isOfType(SoCube::getClassTypeId())) {
                    xform = cubeTransform;
                } else if (n->isOfType(SoCone::getClassTypeId())) {
                    xform = coneTransform;
                } else if (n->isOfType(SoSphere::getClassTypeId())) {
                    xform = sphereTransform;
                } else if (n->isOfType(SoCylinder::getClassTypeId())) {
                    xform = cylTransform;
                }
            }

            if (xform) {
                SbVec3f currentScale = xform->scaleFactor.getValue();
                currentScale *= (1.0f / 1.1f);
                xform->scaleFactor.setValue(currentScale);
            }
        }
        eventCB->setHandled();
    }
}

// Legacy function - kept for reference but no longer used in headless mode
// In the new pattern, we use actual event simulation instead of direct calls
void scaleSelection(SoSelection *selection, float scaleFactor)
{
    for (int i = 0; i < selection->getNumSelected(); i++) {
        SoPath *selectedPath = selection->getPath(i);
        SoTransform *xform = NULL;

        // Look for the shape node to identify which transform to modify
        for (int j = 0; j < selectedPath->getLength() && (xform == NULL); j++) {
            SoNode *n = selectedPath->getNodeFromTail(j);
            if (n->isOfType(SoCube::getClassTypeId())) {
                xform = cubeTransform;
            } else if (n->isOfType(SoCone::getClassTypeId())) {
                xform = coneTransform;
            } else if (n->isOfType(SoSphere::getClassTypeId())) {
                xform = sphereTransform;
            } else if (n->isOfType(SoCylinder::getClassTypeId())) {
                xform = cylTransform;
            }
        }

        if (xform) {
            // Apply the scale
            SbVec3f currentScale = xform->scaleFactor.getValue();
            currentScale *= scaleFactor;
            xform->scaleFactor.setValue(currentScale);
            
            const char *shapeName = "unknown";
            if (xform == cubeTransform) shapeName = "cube";
            else if (xform == sphereTransform) shapeName = "sphere";
            else if (xform == coneTransform) shapeName = "cone";
            else if (xform == cylTransform) shapeName = "cylinder";
            
            printf("Scaled %s by %.2f to (%.2f, %.2f, %.2f)\n", 
                   shapeName, scaleFactor,
                   currentScale[0], currentScale[1], currentScale[2]);
        }
    }
}

int main(int argc, char **argv)
{
    initCoinHeadless();

    // Create and set up the selection node
    SoSelection *selectionRoot = new SoSelection;
    selectionRoot->ref();
    selectionRoot->policy = SoSelection::SHIFT;
    
    // Add camera and light
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    selectionRoot->addChild(myCamera);
    selectionRoot->addChild(new SoDirectionalLight);

    // Event callback node - registers callback for keyboard events
    // This demonstrates the pattern: event callback receives events and processes them
    SoEventCallback *myEventCB = new SoEventCallback;
    myEventCB->addEventCallback(SoKeyboardEvent::getClassTypeId(),
                                 myKeyPressCB, selectionRoot);
    selectionRoot->addChild(myEventCB);

    // Add geometry - a red cube
    SoSeparator *cubeRoot = new SoSeparator;
    cubeTransform = new SoTransform;
    cubeTransform->translation.setValue(-2, 2, 0);
    cubeRoot->addChild(cubeTransform);
    SoMaterial *cubeMaterial = new SoMaterial;
    cubeMaterial->diffuseColor.setValue(.8, 0, 0);
    cubeRoot->addChild(cubeMaterial);
    cubeRoot->addChild(new SoCube);
    selectionRoot->addChild(cubeRoot);

    // A blue sphere
    SoSeparator *sphereRoot = new SoSeparator;
    sphereTransform = new SoTransform;
    sphereTransform->translation.setValue(2, 2, 0);
    sphereRoot->addChild(sphereTransform);
    SoMaterial *sphereMaterial = new SoMaterial;
    sphereMaterial->diffuseColor.setValue(0, 0, .8);
    sphereRoot->addChild(sphereMaterial);
    sphereRoot->addChild(new SoSphere);
    selectionRoot->addChild(sphereRoot);

    // A green cone
    SoSeparator *coneRoot = new SoSeparator;
    coneTransform = new SoTransform;
    coneTransform->translation.setValue(2, -2, 0);
    coneRoot->addChild(coneTransform);
    SoMaterial *coneMaterial = new SoMaterial;
    coneMaterial->diffuseColor.setValue(0, .8, 0);
    coneRoot->addChild(coneMaterial);
    coneRoot->addChild(new SoCone);
    selectionRoot->addChild(coneRoot);

    // A magenta cylinder
    SoSeparator *cylRoot = new SoSeparator;
    cylTransform = new SoTransform;
    cylTransform->translation.setValue(-2, -2, 0);
    cylRoot->addChild(cylTransform);
    SoMaterial *cylMaterial = new SoMaterial;
    cylMaterial->diffuseColor.setValue(.8, 0, .8);
    cylRoot->addChild(cylMaterial);
    cylRoot->addChild(new SoCylinder);
    selectionRoot->addChild(cylRoot);

    // Setup camera
    SbViewportRegion viewport(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    myCamera->viewAll(selectionRoot, viewport, 2.0);

    const char *baseFilename = (argc > 1) ? argv[1] : "10.1.addEventCB";
    char filename[256];

    int frameNum = 0;

    // Render initial state
    printf("\n=== Initial state (nothing selected) ===\n");
    snprintf(filename, sizeof(filename), "%s_frame%02d_initial.rgb", baseFilename, frameNum++);
    renderToFile(selectionRoot, filename);

    // Find and select the cube and sphere
    SoSearchAction search;
    
    search.setType(SoCube::getClassTypeId());
    search.setInterest(SoSearchAction::FIRST);
    search.apply(selectionRoot);
    SoPath *cubePath = search.getPath();
    if (cubePath) {
        cubePath = cubePath->copy();
        cubePath->ref();
        selectionRoot->select(cubePath);
        printf("Selected cube\n");
    }

    search.reset();
    search.setType(SoSphere::getClassTypeId());
    search.setInterest(SoSearchAction::FIRST);
    search.apply(selectionRoot);
    SoPath *spherePath = search.getPath();
    if (spherePath) {
        spherePath = spherePath->copy();
        spherePath->ref();
        selectionRoot->select(spherePath);
        printf("Selected sphere\n");
    }

    // Render with selections
    printf("\n=== Cube and sphere selected ===\n");
    snprintf(filename, sizeof(filename), "%s_frame%02d_selected.rgb", baseFilename, frameNum++);
    renderToFile(selectionRoot, filename);

    // Simulate UP ARROW key presses (scale up)
    // Using the new event simulation pattern from manipulator work
    printf("\n=== Simulating UP ARROW key presses (scale up) ===\n");
    printf("This demonstrates event simulation triggering callbacks\n");
    for (int i = 0; i < 3; i++) {
        // Simulate key press event - this will trigger myKeyPressCB callback
        simulateKeyPress(selectionRoot, viewport, SoKeyboardEvent::UP_ARROW);
        
        // Process sensor queues to ensure callbacks complete
        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(TRUE);
        
        // Simulate key release
        simulateKeyRelease(selectionRoot, viewport, SoKeyboardEvent::UP_ARROW);
        
        snprintf(filename, sizeof(filename), "%s_frame%02d_scaleup_%d.rgb", baseFilename, frameNum++, i+1);
        renderToFile(selectionRoot, filename);
    }

    // Simulate DOWN ARROW key presses (scale down)
    printf("\n=== Simulating DOWN ARROW key presses (scale down) ===\n");
    for (int i = 0; i < 5; i++) {
        // Simulate key press event - this will trigger myKeyPressCB callback
        simulateKeyPress(selectionRoot, viewport, SoKeyboardEvent::DOWN_ARROW);
        
        // Process sensor queues to ensure callbacks complete
        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(TRUE);
        
        // Simulate key release
        simulateKeyRelease(selectionRoot, viewport, SoKeyboardEvent::DOWN_ARROW);
        
        snprintf(filename, sizeof(filename), "%s_frame%02d_scaledown_%d.rgb", baseFilename, frameNum++, i+1);
        renderToFile(selectionRoot, filename);
    }

    printf("\nRendered %d frames demonstrating event callbacks\n", frameNum);
    printf("Events were simulated using the new manipulator pattern:\n");
    printf("  - simulateKeyPress/Release from headless_utils.h\n");
    printf("  - Events trigger registered callbacks (myKeyPressCB)\n");
    printf("  - Callbacks scale selected objects based on key\n");

    if (cubePath) cubePath->unref();
    if (spherePath) spherePath->unref();
    selectionRoot->unref();
    return 0;
}
