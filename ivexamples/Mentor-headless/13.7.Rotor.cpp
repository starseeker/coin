/*
 *
 *  Copyright (C) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  Further, this software is distributed without any warranty that it is
 *  free of the rightful claim of any third person regarding infringement
 *  or the like.  Any license provided herein, whether implied or
 *  otherwise, applies only to this software file.  Patent licenses, if
 *  any, provided herein do not apply to combinations of this program with
 *  other software, or any other product whatsoever.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 *  Mountain View, CA  94043, or:
 *
 *  http://www.sgi.com
 *
 *  For further information regarding this notice, see:
 *
 *  http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 *
 */

/*
 * Headless version of Inventor Mentor example 13.7
 * 
 * Original: Rotor - rotating windmill vanes
 * Headless: Renders rotation sequence at different angles
 */

#include "headless_utils.h"
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoRotor.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <cstdio>
#include <cmath>

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

    // Create a windmill tower: a slightly tapered cylinder body with a base
    SoMaterial *towerMat = new SoMaterial;
    towerMat->diffuseColor.setValue(0.5, 0.3, 0.1);
    root->addChild(towerMat);

    SoTransform *towerXform = new SoTransform;
    towerXform->translation.setValue(0, -1.5, 0);
    towerXform->scaleFactor.setValue(0.5, 3.0, 0.5);
    root->addChild(towerXform);
    root->addChild(new SoCylinder);

    // Add a rotor node to spin the vanes
    SoRotor *myRotor = new SoRotor;
    myRotor->rotation.setValue(SbVec3f(0, 0, 1), 0); // z axis
    myRotor->speed = 0.2;
    root->addChild(myRotor);

    // Create windmill vanes: 4 rectangular blades arranged in a cross pattern.
    // Use a hub (small cylinder) plus 4 blade cubes with visible depth so they
    // look like real vanes and not thin lines from any camera angle.
    SoSeparator *vanesSep = new SoSeparator;
    root->addChild(vanesSep);

    SoMaterial *vanesMat = new SoMaterial;
    vanesMat->diffuseColor.setValue(0.8, 0.75, 0.5);
    vanesSep->addChild(vanesMat);

    // Hub
    SoSeparator *hubSep = new SoSeparator;
    vanesSep->addChild(hubSep);
    SoTransform *hubXform = new SoTransform;
    hubXform->scaleFactor.setValue(0.3, 0.3, 0.3);
    hubSep->addChild(hubXform);
    hubSep->addChild(new SoCylinder);

    // Four blades at 0°, 90°, 180°, 270°
    const float bladeAngles[4] = {0.0f, (float)M_PI/2, (float)M_PI, 3*(float)M_PI/2};
    for (int b = 0; b < 4; b++) {
        SoSeparator *bladeSep = new SoSeparator;
        vanesSep->addChild(bladeSep);

        SoTransform *bladeXform = new SoTransform;
        // Place blade centre at radius 1.2 from hub
        float bx = 1.2f * cosf(bladeAngles[b]);
        float by = 1.2f * sinf(bladeAngles[b]);
        bladeXform->translation.setValue(bx, by, 0);
        bladeXform->rotation.setValue(SbVec3f(0, 0, 1), bladeAngles[b]);
        // Blade: long in Y (2.0), medium width in X (0.4), visible depth in Z (0.15)
        bladeXform->scaleFactor.setValue(0.4f, 2.0f, 0.15f);
        bladeSep->addChild(bladeXform);
        bladeSep->addChild(new SoCube);
    }

    // Position camera at an elevated angle to show the windmill in 3D.
    // Viewing from upper-left rather than straight-on makes the vane depth
    // visible and the overall structure look like a rotor.
    camera->position.setValue(4.0f, 3.0f, 8.0f);
    camera->pointAt(SbVec3f(0, 0, 0), SbVec3f(0, 1, 0));

    const char *baseFilename = (argc > 1) ? argv[1] : "13.7.Rotor";
    char filename[256];

    // Enable the rotor
    myRotor->on = TRUE;

    // Render rotation sequence
    const float ROTATION_INCREMENT = M_PI / 6.0;  // 30 degrees per frame
    for (int i = 0; i <= 12; i++) {
        // Manually advance the rotor rotation
        float angle = i * ROTATION_INCREMENT;
        myRotor->rotation.setValue(SbVec3f(0, 0, 1), angle);
        
        // Process any pending updates
        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(TRUE);
        
        printf("Frame %d: Rotation angle = %.1f degrees\n", i, angle * 180.0 / M_PI);
        
        // Render this frame
        snprintf(filename, sizeof(filename), "%s_frame%02d.rgb", baseFilename, i);
        renderToFile(root, filename);
    }

    root->unref();
    return 0;
}
