/*
 * Headless version of Inventor Mentor example 3.1
 * 
 * Original: Molecule - creates a water molecule with interactive viewing
 * Headless: Renders a water molecule to image files from multiple angles
 */

#include "headless_utils.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <cmath>
#include <cstdio>
#include <cstring>

SoGroup *makeWaterMolecule()
{
    // Construct all parts
    SoGroup *waterMolecule = new SoGroup;  // water molecule

    SoGroup *oxygen = new SoGroup;         // oxygen atom
    SoMaterial *redPlastic = new SoMaterial;
    SoSphere *sphere1 = new SoSphere;

    SoGroup *hydrogen1 = new SoGroup;      // hydrogen atoms
    SoGroup *hydrogen2 = new SoGroup;
    SoTransform *hydrogenXform1 = new SoTransform;
    SoTransform *hydrogenXform2 = new SoTransform;
    SoMaterial *whitePlastic = new SoMaterial;

    // Set all field values for the oxygen atom
    redPlastic->ambientColor.setValue(1.0, 0.0, 0.0);  
    redPlastic->diffuseColor.setValue(1.0, 0.0, 0.0); 
    redPlastic->specularColor.setValue(0.5, 0.5, 0.5);
    redPlastic->shininess = 0.5;
    
    // Set all field values for the hydrogen atoms
    hydrogenXform1->scaleFactor.setValue(0.75, 0.75, 0.75);  
    hydrogenXform1->translation.setValue(0.0, -1.2, 0.0);  
    hydrogenXform2->translation.setValue(1.1852, 1.3877, 0.0);
    whitePlastic->ambientColor.setValue(1.0, 1.0, 1.0);  
    whitePlastic->diffuseColor.setValue(1.0, 1.0, 1.0); 
    whitePlastic->specularColor.setValue(0.5, 0.5, 0.5);
    whitePlastic->shininess = 0.5;

    // Create a hierarchy
    waterMolecule->addChild(oxygen);   
    waterMolecule->addChild(hydrogen1);   
    waterMolecule->addChild(hydrogen2);

    oxygen->addChild(redPlastic);
    oxygen->addChild(sphere1);
    hydrogen1->addChild(hydrogenXform1);
    hydrogen1->addChild(whitePlastic);
    hydrogen1->addChild(new SoSphere);
    hydrogen2->addChild(hydrogenXform2);
    hydrogen2->addChild(new SoSphere);

    return waterMolecule;
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

    // Add the water molecule
    root->addChild(makeWaterMolecule());

    // Setup camera
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    // Render from multiple angles
    const char *baseFilename = (argc > 1) ? argv[1] : "03.1.Molecule";
    char filename[256];
    
    // Front view
    snprintf(filename, sizeof(filename), "%s_front.rgb", baseFilename);
    renderToFile(root, filename);
    
    // Rotate camera for side view
    rotateCamera(camera, M_PI / 2, 0);
    snprintf(filename, sizeof(filename), "%s_side.rgb", baseFilename);
    renderToFile(root, filename);
    
    // Rotate for top view
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));
    rotateCamera(camera, 0, M_PI / 4);
    snprintf(filename, sizeof(filename), "%s_angle.rgb", baseFilename);
    renderToFile(root, filename);

    root->unref();
    return 0;
}
