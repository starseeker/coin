/*
 * Headless version of Inventor Mentor example 12.3
 * 
 * Original: AlarmSensor - raises a flag after 12 seconds
 * Headless: Simulates alarm trigger and renders before/after states
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/SbTime.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <cstdio>

bool flagRaised = false;

static void
raiseFlagCallback(void *data, SoSensor *)
{
    // We know data is really a SoTransform node
    SoTransform *flagAngleXform = (SoTransform *)data;

    // Rotate flag by 90 degrees about the Z axis
    flagAngleXform->rotation.setValue(SbVec3f(0, 0, 1), M_PI/2);
    
    flagRaised = true;
    printf("Alarm triggered! Flag raised.\n");
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

    // Create transform for the flag/cone
    SoTransform *flagXform = new SoTransform;
    root->addChild(flagXform);
    
    // Add a cone to represent the flag
    SoCone *myCone = new SoCone;
    myCone->bottomRadius = 0.1;
    root->addChild(myCone);

    // Set up camera
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    // Create an alarm that will call the flag-raising callback
    SoAlarmSensor *myAlarm = new SoAlarmSensor(raiseFlagCallback, flagXform);
    myAlarm->setTime(SbTime::getTimeOfDay() + 0.1);  // Set to trigger soon
    myAlarm->schedule();

    const char *baseFilename = (argc > 1) ? argv[1] : "12.3.AlarmSensor";
    char filename[256];

    // Render before alarm triggers
    printf("Before alarm triggers...\n");
    snprintf(filename, sizeof(filename), "%s_before.rgb", baseFilename);
    renderToFile(root, filename);

    // Process the sensor queue to trigger the alarm
    printf("\nProcessing sensor queue...\n");
    SoDB::getSensorManager()->processTimerQueue();
    
    // Render after alarm triggers
    printf("\nAfter alarm triggers...\n");
    snprintf(filename, sizeof(filename), "%s_after.rgb", baseFilename);
    renderToFile(root, filename);

    printf("\nFlag raised: %s\n", flagRaised ? "Yes" : "No");

    delete myAlarm;
    root->unref();

    return 0;
}
