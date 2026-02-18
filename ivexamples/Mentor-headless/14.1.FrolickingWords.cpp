/*
 *  Headless version of Inventor Mentor example 14.1
 *
 *  Converted from interactive viewer to headless rendering.
 *  Uses SoShapeKits to create two 3-D words, "NICE" and "HAPPY"
 *  Uses a calculator engine and an elapsed time engine to make
 *  the words change color and fly about.
 *  Renders animation frames at different time values.
 */

#include "headless_utils.h"
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <cstdio>

int
main(int, char **)
{
   initCoinHeadless();

   SoSeparator *root = new SoSeparator;
   root->ref();

   // Add camera and light for headless rendering
   SoPerspectiveCamera *camera = new SoPerspectiveCamera;
   camera->position.setValue(0, 0, 15);
   camera->orientation.setValue(SbRotation(SbVec3f(0, 1, 0), 0));
   root->addChild(camera);

   SoDirectionalLight *light = new SoDirectionalLight;
   root->addChild(light);

   // Create shape kits with the words "HAPPY" and "NICE"
   SoShapeKit *happyKit = new SoShapeKit;
   root->addChild(happyKit);
   happyKit->setPart("shape", new SoText3);
   happyKit->set("shape { parts ALL string \"HAPPY\"}");
   happyKit->set("font { size 2}");

   SoShapeKit *niceKit = new SoShapeKit;
   root->addChild(niceKit);
   niceKit->setPart("shape", new SoText3);
   niceKit->set("shape { parts ALL string \"NICE\"}");
   niceKit->set("font { size 2}");

   // Create the Elapsed Time engine
   SoElapsedTime *myTimer = new SoElapsedTime;
   myTimer->ref();

   // Create two calculator - one for HAPPY, one for NICE.
   SoCalculator *happyCalc = new SoCalculator;
   happyCalc->ref();
   happyCalc->a.connectFrom(&myTimer->timeOut);
   happyCalc->expression = "ta=cos(2*a); tb=sin(2*a); \
     oA = vec3f(3*pow(ta,3),3*pow(tb,3),1); \
     oB = vec3f(fabs(ta)+.1,fabs(tb)/2+.1,1); \
     oC = vec3f(fabs(ta),fabs(tb),.5)";

   // The second calculator uses different arguments to
   // sin() and cos(), so it moves out of phase.
   SoCalculator *niceCalc = new SoCalculator;
   niceCalc->ref();
   niceCalc->a.connectFrom(&myTimer->timeOut);
   niceCalc->expression = "ta=cos(2*a+2); tb=sin(2*a+2); \
     oA = vec3f(3*pow(ta,3),3*pow(tb,3),1); \
     oB = vec3f(fabs(ta)+.1,fabs(tb)/2+.1,1); \
     oC = vec3f(fabs(ta),fabs(tb),.5)";

   // Connect the transforms from the calculators...
   SoTransform *happyXf
      = (SoTransform *) happyKit->getPart("transform",TRUE);
   happyXf->translation.connectFrom(&happyCalc->oA);
   happyXf->scaleFactor.connectFrom(&happyCalc->oB);
   SoTransform *niceXf
      = (SoTransform *) niceKit->getPart("transform",TRUE);
   niceXf->translation.connectFrom(&niceCalc->oA);
   niceXf->scaleFactor.connectFrom(&niceCalc->oB);

   // Connect the materials from the calculators...
   SoMaterial *happyMtl
      = (SoMaterial *) happyKit->getPart("material",TRUE);
   happyMtl->diffuseColor.connectFrom(&happyCalc->oC);
   SoMaterial *niceMtl
      = (SoMaterial *) niceKit->getPart("material",TRUE);
   niceMtl->diffuseColor.connectFrom(&niceCalc->oC);

   // Render animation sequence at different time values
   char filename[64];
   SbViewportRegion viewport(DEFAULT_WIDTH, DEFAULT_HEIGHT);
   
   printf("Rendering Frolicking Words animation sequence...\n");
   
   // Render frames at various times to capture the animation
   for (int frame = 0; frame < 20; frame++) {
      float time = frame * 0.4f;  // 0.4 second intervals
      
      // Set the time explicitly
      myTimer->timeIn.setValue(SbTime(time));
      
      // Process the sensor queue to let engines update
      SoDB::getSensorManager()->processTimerQueue();
      SoDB::getSensorManager()->processDelayQueue(TRUE);
      
      sprintf(filename, "output/14.1.FrolickingWords_%02d.rgb", frame);
      renderToFile(root, filename, DEFAULT_WIDTH, DEFAULT_HEIGHT);
   }
   
   printf("Done! Rendered 20 animation frames.\n");

   myTimer->unref();
   happyCalc->unref();
   niceCalc->unref();
   root->unref();

   return 0;
}
