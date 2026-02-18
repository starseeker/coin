/*
 * Headless version of Inventor Mentor example 8.1
 * 
 * Original: BSCurve - displays B-spline curve with floor and shadow
 * Headless: Renders B-spline curve from multiple angles
 */

#include "headless_utils.h"
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <cmath>
#include <cstdio>

// The control points for this curve
const float pts[7][3] = {
   { 4.0, -6.0,  6.0},
   {-4.0,  1.0,  0.0},
   {-1.5,  5.0, -6.0},
   { 0.0,  2.0, -2.0},
   { 1.5,  5.0, -6.0},
   { 4.0,  1.0,  0.0},
   {-4.0, -6.0,  6.0}};

// The knot vector
float knots[10] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};

// Create the nodes needed for the B-Spline curve
SoSeparator *makeCurve()
{
    SoSeparator *curveSep = new SoSeparator();
    curveSep->ref();

    // Set the draw style of the curve
    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->lineWidth = 4;
    curveSep->addChild(drawStyle);

    // Define the NURBS curve including the control points and complexity
    SoComplexity *complexity = new SoComplexity;
    SoCoordinate3 *controlPts = new SoCoordinate3;
    SoNurbsCurve *curve = new SoNurbsCurve;
    complexity->value = 0.8;
    controlPts->point.setValues(0, 7, pts);
    curve->numControlPoints = 7;
    curve->knotVector.setValues(0, 10, knots);
    curveSep->addChild(complexity);
    curveSep->addChild(controlPts);
    curveSep->addChild(curve);

    curveSep->unrefNoDelete();
    return curveSep;
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

    // Create the scene graph for the curve
    SoSeparator *heart = new SoSeparator;
    SoSeparator *curveSep = makeCurve();
    SoLightModel *lmodel = new SoLightModel;
    SoBaseColor *clr = new SoBaseColor;

    lmodel->model = SoLightModel::BASE_COLOR;
    clr->rgb.setValue(SbColor(1.0, 0.0, 0.1));
    heart->addChild(lmodel);
    heart->addChild(clr);
    heart->addChild(curveSep);
    root->addChild(heart);

    // Setup camera
    camera->position.setValue(SbVec3f(-6.0, 8.0, 20.0));
    camera->pointAt(SbVec3f(0.0, -2.0, -4.0));

    const char *baseFilename = (argc > 1) ? argv[1] : "08.1.BSCurve";
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
