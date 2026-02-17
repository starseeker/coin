/*
 * Headless version of Inventor Mentor example 6.1
 * 
 * Original: Text - renders globe with 2D text labels
 * Headless: Renders sphere with 2D text from multiple angles
 */

#include "headless_utils.h"
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <cmath>
#include <cstdio>

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    SoGroup *root = new SoGroup;
    root->ref();

    // Add camera and light
    SoPerspectiveCamera *camera = new SoPerspectiveCamera;
    root->addChild(camera);
    root->addChild(new SoDirectionalLight);

    // Choose a font
    SoFont *myFont = new SoFont;
    myFont->name.setValue("Times");
    myFont->size.setValue(24.0);
    root->addChild(myFont);

    // Add the globe - a sphere
    SoSeparator *sphereSep = new SoSeparator;
    SoComplexity *sphereComplexity = new SoComplexity;
    sphereComplexity->value = 0.55;
    root->addChild(sphereSep);
    sphereSep->addChild(sphereComplexity);
    sphereSep->addChild(new SoSphere);

    // Add Text2 for AFRICA
    SoSeparator *africaSep = new SoSeparator;
    SoTranslation *africaTranslate = new SoTranslation;
    SoText2 *africaText = new SoText2;
    africaTranslate->translation.setValue(.25, .0, 1.25);
    africaText->string = "AFRICA";
    root->addChild(africaSep);
    africaSep->addChild(africaTranslate);
    africaSep->addChild(africaText);

    // Add Text2 for ASIA
    SoSeparator *asiaSep = new SoSeparator;
    SoTranslation *asiaTranslate = new SoTranslation;
    SoText2 *asiaText = new SoText2;
    asiaTranslate->translation.setValue(.8, .6, .5);
    asiaText->string = "ASIA";
    root->addChild(asiaSep);
    asiaSep->addChild(asiaTranslate);
    asiaSep->addChild(asiaText);

    // Setup camera
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    const char *baseFilename = (argc > 1) ? argv[1] : "06.1.Text";
    char filename[256];

    // Front view
    snprintf(filename, sizeof(filename), "%s_front.rgb", baseFilename);
    renderToFile(root, filename);

    // Rotated view
    rotateCamera(camera, M_PI / 4, M_PI / 6);
    snprintf(filename, sizeof(filename), "%s_angle.rgb", baseFilename);
    renderToFile(root, filename);

    root->unref();
    return 0;
}
