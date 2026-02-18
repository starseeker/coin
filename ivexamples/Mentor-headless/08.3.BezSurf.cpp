/*
 * Headless version of Inventor Mentor example 8.3
 * 
 * Original: BezSurf - displays Bezier surface with floor
 * Headless: Renders Bezier surface from multiple angles
 */

#include "headless_utils.h"
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNurbsSurface.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <cmath>
#include <cstdio>

// The control points for this surface
const float pts[16][3] = {
   {-4.5, -2.0,  8.0},
   {-2.0,  1.0,  8.0},
   { 2.0, -3.0,  6.0},
   { 5.0, -1.0,  8.0},
   {-3.0,  3.0,  4.0},
   { 0.0, -1.0,  4.0},
   { 1.0, -1.0,  4.0},
   { 3.0,  2.0,  4.0},
   {-5.0, -2.0, -2.0},
   {-2.0, -4.0, -2.0},
   { 2.0, -1.0, -2.0},
   { 5.0,  0.0, -2.0},
   {-4.5,  2.0, -6.0},
   {-2.0, -4.0, -5.0},
   { 2.0,  3.0, -5.0},
   { 4.5, -2.0, -6.0}};

// The knot vector
float knots[8] = {
   0, 0, 0, 0, 1, 1, 1, 1};

// Create the nodes needed for the Bezier surface
SoSeparator *makeSurface()
{
    SoSeparator *surfSep = new SoSeparator();
    surfSep->ref();

    // Define the Bezier surface including the control points and complexity
    SoComplexity *complexity = new SoComplexity;
    SoCoordinate3 *controlPts = new SoCoordinate3;
    SoNurbsSurface *surface = new SoNurbsSurface;
    complexity->value = 0.7;
    controlPts->point.setValues(0, 16, pts);
    surface->numUControlPoints = 4;
    surface->numVControlPoints = 4;
    surface->uKnotVector.setValues(0, 8, knots);
    surface->vKnotVector.setValues(0, 8, knots);
    surfSep->addChild(complexity);
    surfSep->addChild(controlPts);
    surfSep->addChild(surface);

    surfSep->unrefNoDelete();
    return surfSep;
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

    // Add material for the surface
    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue(0.8, 0.3, 0.1);
    root->addChild(mat);

    // Create the Bezier surface
    SoSeparator *surfSep = makeSurface();
    root->addChild(surfSep);

    // Setup camera
    camera->position.setValue(SbVec3f(-6.0, 8.0, 20.0));
    camera->pointAt(SbVec3f(0.0, 0.0, 0.0));

    const char *baseFilename = (argc > 1) ? argv[1] : "08.3.BezSurf";
    char filename[256];

    // View from camera position
    snprintf(filename, sizeof(filename), "%s_view1.rgb", baseFilename);
    renderToFile(root, filename);

    // Side view
    camera->position.setValue(SbVec3f(20.0, 0.0, 0.0));
    camera->pointAt(SbVec3f(0.0, 0.0, 0.0));
    snprintf(filename, sizeof(filename), "%s_side.rgb", baseFilename);
    renderToFile(root, filename);

    // Top view
    camera->position.setValue(SbVec3f(0.0, 20.0, 0.0));
    camera->pointAt(SbVec3f(0.0, 0.0, 0.0));
    snprintf(filename, sizeof(filename), "%s_top.rgb", baseFilename);
    renderToFile(root, filename);

    root->unref();
    return 0;
}
