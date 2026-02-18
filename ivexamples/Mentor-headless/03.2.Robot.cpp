/*
 * Headless version of Inventor Mentor example 3.2
 * 
 * Original: Robot - creates a robot using node sharing for legs
 * Headless: Renders the robot from multiple viewpoints
 */

#include "headless_utils.h"
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <cmath>
#include <cstdio>

SoSeparator *makeRobot()
{
    // Robot with legs

    // Construct parts for legs (thigh, calf and foot)
    SoCube *thigh = new SoCube;     
    thigh->width = 1.2;
    thigh->height = 2.2;
    thigh->depth = 1.1;

    SoTransform *calfTransform = new SoTransform;
    calfTransform->translation.setValue(0, -2.25, 0.0);

    SoCube *calf = new SoCube;
    calf->width = 1;
    calf->height = 2.2;
    calf->depth = 1;

    SoTransform *footTransform = new SoTransform;
    footTransform->translation.setValue(0, -1.5, .5);

    SoCube *foot = new SoCube;
    foot->width = 0.8;
    foot->height = 0.8;
    foot->depth = 2;

    // Put leg parts together
    SoGroup *leg = new SoGroup;      
    leg->addChild(thigh);
    leg->addChild(calfTransform);
    leg->addChild(calf);
    leg->addChild(footTransform);
    leg->addChild(foot);

    SoTransform *leftTransform = new SoTransform;
    leftTransform->translation = SbVec3f(1, -4.25, 0);

    // Left leg (shared instance)
    SoSeparator *leftLeg = new SoSeparator;   
    leftLeg->addChild(leftTransform);
    leftLeg->addChild(leg);

    SoTransform *rightTransform = new SoTransform;
    rightTransform->translation.setValue(-1, -4.25, 0);

    // Right leg (shared instance)
    SoSeparator *rightLeg = new SoSeparator;   
    rightLeg->addChild(rightTransform);
    rightLeg->addChild(leg);

    // Parts for body
    SoTransform *bodyTransform = new SoTransform;    
    bodyTransform->translation.setValue(0.0, 3.0, 0.0);

    SoMaterial *bronze = new SoMaterial;
    bronze->ambientColor.setValue(.33, .22, .27);
    bronze->diffuseColor.setValue(.78, .57, .11);
    bronze->specularColor.setValue(.99, .94, .81);
    bronze->shininess = .28;

    SoCylinder *bodyCylinder = new SoCylinder;
    bodyCylinder->radius = 2.5;
    bodyCylinder->height = 6;

    // Construct body out of parts 
    SoSeparator *body = new SoSeparator;  
    body->addChild(bodyTransform);      
    body->addChild(bronze);
    body->addChild(bodyCylinder);
    body->addChild(leftLeg);
    body->addChild(rightLeg);

    // Head parts
    SoTransform *headTransform = new SoTransform;   
    headTransform->translation.setValue(0, 7.5, 0);
    headTransform->scaleFactor.setValue(1.5, 1.5, 1.5);

    SoMaterial *silver = new SoMaterial;
    silver->ambientColor.setValue(.2, .2, .2);
    silver->diffuseColor.setValue(.6, .6, .6);
    silver->specularColor.setValue(.5, .5, .5);
    silver->shininess = .5;

    SoSphere *headSphere = new SoSphere;

    // Construct head
    SoSeparator *head = new SoSeparator;      
    head->addChild(headTransform);
    head->addChild(silver);
    head->addChild(headSphere);
    
    // Robot is just head and body
    SoSeparator *robot = new SoSeparator;      
    robot->addChild(body);
    robot->addChild(head);

    return robot;
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

    // Add the robot
    root->addChild(makeRobot());

    // Setup camera
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    // Render from multiple angles
    const char *baseFilename = (argc > 1) ? argv[1] : "03.2.Robot";
    char filename[256];
    
    // Front view
    snprintf(filename, sizeof(filename), "%s_front.rgb", baseFilename);
    renderToFile(root, filename);
    
    // Side view
    rotateCamera(camera, M_PI / 2, 0);
    snprintf(filename, sizeof(filename), "%s_side.rgb", baseFilename);
    renderToFile(root, filename);
    
    // 45 degree angle view
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));
    rotateCamera(camera, M_PI / 4, M_PI / 6);
    snprintf(filename, sizeof(filename), "%s_angle.rgb", baseFilename);
    renderToFile(root, filename);

    root->unref();
    return 0;
}
