/*
 * Headless version of Inventor Mentor example 6.3
 * 
 * Original: Complex3DText - renders fancy 3D text with profiles
 * Headless: Renders 3D text with beveled cross-section
 */

#include "headless_utils.h"
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoLinearProfile.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoProfileCoordinate2.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <cmath>
#include <cstdio>

int main(int argc, char **argv)
{
    // Initialize Coin for headless operation
    initCoinHeadless();

    SoGroup *root = new SoGroup;
    root->ref();

    // Set up camera
    SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
    myCamera->position.setValue(0, -1, 10);
    myCamera->nearDistance.setValue(5.0);
    myCamera->farDistance.setValue(15.0);
    root->addChild(myCamera);

    // Add light
    root->addChild(new SoDirectionalLight);

    // Material: white front, shiny yellow sides/back
    SoMaterial *myMaterial = new SoMaterial;
    SbColor colors[3];
    colors[0].setValue(1, 1, 1);  // diffuse front
    colors[1].setValue(1, 1, 0);  // diffuse sides
    colors[2].setValue(1, 1, 0);  // diffuse back
    myMaterial->diffuseColor.setValues(0, 3, colors);
    myMaterial->specularColor.setValue(1, 1, 1);
    myMaterial->shininess.setValue(.1);
    root->addChild(myMaterial);

    // Material binding
    SoMaterialBinding *myBinding = new SoMaterialBinding;
    myBinding->value = SoMaterialBinding::PER_PART;
    root->addChild(myBinding);

    // Font
    SoFont *myFont = new SoFont;
    myFont->name.setValue("Times-Roman");
    root->addChild(myFont);

    // Beveled cross-section profile
    SoProfileCoordinate2 *myProfileCoords = new SoProfileCoordinate2;
    SbVec2f coords[4];
    coords[0].setValue(.00, .00);
    coords[1].setValue(.25, .25);
    coords[2].setValue(1.25, .25);
    coords[3].setValue(1.50, .00);
    myProfileCoords->point.setValues(0, 4, coords);
    root->addChild(myProfileCoords);

    SoLinearProfile *myLinearProfile = new SoLinearProfile;
    int32_t indices[4] = {0, 1, 2, 3};
    myLinearProfile->index.setValues(0, 4, indices);
    root->addChild(myLinearProfile);

    // Text
    const char *words[] = {"Beveled", "Text"};
    for (int i = 0; i < 2; i++) {
        SoSeparator *textSep = new SoSeparator;
        SoTranslation *myTranslation = new SoTranslation;
        myTranslation->translation.setValue(0, -2.0 * i, 0);
        textSep->addChild(myTranslation);
        
        SoText3 *myText = new SoText3;
        myText->string.setValue(words[i]);
        myText->parts = SoText3::ALL;
        myText->justification = SoText3::CENTER;
        textSep->addChild(myText);
        
        root->addChild(textSep);
    }

    const char *baseFilename = (argc > 1) ? argv[1] : "06.3.Complex3DText";
    char filename[256];

    // Front view
    snprintf(filename, sizeof(filename), "%s_front.rgb", baseFilename);
    renderToFile(root, filename);

    // Angle view
    rotateCamera(myCamera, M_PI/4, M_PI/6);
    snprintf(filename, sizeof(filename), "%s_angle.rgb", baseFilename);
    renderToFile(root, filename);

    root->unref();
    return 0;
}
