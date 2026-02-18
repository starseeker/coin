/*
 * Headless version of Inventor Mentor example 5.3
 * 
 * Original: TriangleStripSet - creates a pennant-shaped flag
 * Headless: Renders the flag from multiple angles
 */

#include "headless_utils.h"
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <cmath>
#include <cstdio>

// Positions of all vertices
static const float vertexPositions[40][3] =
{
   {  0,   12,    0 }, {   0,   15,    0},
   {2.1, 12.1,  -.2 }, { 2.1, 14.6,  -.2},
   {  4, 12.5,  -.7 }, {   4, 14.5,  -.7},
   {4.5, 12.6,  -.8 }, { 4.5, 14.4,  -.8},
   {  5, 12.7,   -1 }, {   5, 14.4,   -1},
   {4.5, 12.8, -1.4 }, { 4.5, 14.6, -1.4},
   {  4, 12.9, -1.6 }, {   4, 14.8, -1.6},
   {3.3, 12.9, -1.8 }, { 3.3, 14.9, -1.8},
   {  3,   13, -2.0 }, {   3, 14.9, -2.0}, 
   {3.3, 13.1, -2.2 }, { 3.3, 15.0, -2.2},
   {  4, 13.2, -2.5 }, {   4, 15.0, -2.5},
   {  6, 13.5, -2.2 }, {   6, 14.8, -2.2},
   {  8, 13.4,   -2 }, {   8, 14.6,   -2},
   { 10, 13.7, -1.8 }, {  10, 14.4, -1.8},
   { 12,   14, -1.3 }, {  12, 14.5, -1.3},
   { 15, 14.9, -1.2 }, {  15,   15, -1.2},

   {-.5, 15,   0 }, { -.5, 0,   0},   // the flagpole
   {  0, 15,  .5 }, {   0, 0,  .5},
   {  0, 15, -.5 }, {   0, 0, -.5},
   {-.5, 15,   0 }, { -.5, 0,   0}
};

// Number of vertices in each strip
static int32_t numVertices[2] = { 32, 8 };  // flag, pole

// Colors for the strips
static const float colors[2][3] =
{
   { .5, .5,  1 }, // purple flag
   { .4, .4, .4 }, // grey flagpole
};

SoSeparator *makePennant()
{
    SoSeparator *result = new SoSeparator;
    result->ref();

    // Shape hints for double sided lighting
    SoShapeHints *myHints = new SoShapeHints;
    myHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    result->addChild(myHints);

    // Define material binding
    SoMaterialBinding *myBinding = new SoMaterialBinding;
    myBinding->value = SoMaterialBinding::PER_PART;
    result->addChild(myBinding);

    // Define materials
    SoMaterial *myMaterials = new SoMaterial;
    myMaterials->diffuseColor.setValues(0, 2, colors);
    result->addChild(myMaterials);

    // Define coordinates
    SoCoordinate3 *myCoords = new SoCoordinate3;
    myCoords->point.setValues(0, 40, vertexPositions);
    result->addChild(myCoords);

    // Define the TriangleStripSet
    SoTriangleStripSet *myStrips = new SoTriangleStripSet;
    myStrips->numVertices.setValues(0, 2, numVertices);
    result->addChild(myStrips);

    result->unrefNoDelete();
    return result;
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

    root->addChild(makePennant());

    // Setup camera
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    const char *baseFilename = (argc > 1) ? argv[1] : "05.3.TriangleStripSet";
    char filename[256];

    // Front view
    snprintf(filename, sizeof(filename), "%s_front.rgb", baseFilename);
    renderToFile(root, filename);

    // Side view
    rotateCamera(camera, M_PI / 2, 0);
    snprintf(filename, sizeof(filename), "%s_side.rgb", baseFilename);
    renderToFile(root, filename);

    // Angled view
    camera->viewAll(root, SbViewportRegion(DEFAULT_WIDTH, DEFAULT_HEIGHT));
    rotateCamera(camera, M_PI / 4, M_PI / 8);
    snprintf(filename, sizeof(filename), "%s_angle.rgb", baseFilename);
    renderToFile(root, filename);

    root->unref();
    return 0;
}
