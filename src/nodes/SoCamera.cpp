/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoCamera SoCamera.h Inventor/nodes/SoCamera.h
  \brief The SoCamera class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoCamera.h>



#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
#include <Inventor/actions/SoRayPickAction.h>
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#if !defined(COIN_EXCLUDE_SOGLPROJECTIONMATRIXELEMENT)
#include <Inventor/elements/SoGLProjectionMatrixElement.h>
#endif // !COIN_EXCLUDE_SOGLPROJECTIONMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLVIEWINGMATRIXELEMENT)
#include <Inventor/elements/SoGLViewingMatrixElement.h>
#endif // !COIN_EXCLUDE_SOGLVIEWINGMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT)
#include <Inventor/elements/SoGLViewportRegionElement.h>
#endif // !COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT
#if !defined(COIN_EXCLUDE_SOGLUPDATEAREAELEMENT)
#include <Inventor/elements/SoGLUpdateAreaElement.h>
#endif // !COIN_EXCLUDE_SOGLUPDATEAREAELEMENT
#if !defined(COIN_EXCLUDE_SOFOCALDISTANCEELEMENT)
#include <Inventor/elements/SoFocalDistanceElement.h>
#endif // !COIN_EXCLUDE_SOFOCALDISTANCEELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWVOLUMEELEMENT)
#include <Inventor/elements/SoViewVolumeElement.h>
#endif // !COIN_EXCLUDE_SOVIEWVOLUMEELEMENT

#include <Inventor/elements/SoModelMatrixElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <assert.h>

/*!
  \enum SoCamera::ViewportMapping
  FIXME: write documentation for enum
*/
/*!
  \var SoCamera::ViewportMapping SoCamera::CROP_VIEWPORT_FILL_FRAME
  FIXME: write documentation for enum definition
*/
/*!
  \var SoCamera::ViewportMapping SoCamera::CROP_VIEWPORT_LINE_FRAME
  FIXME: write documentation for enum definition
*/
/*!
  \var SoCamera::ViewportMapping SoCamera::CROP_VIEWPORT_NO_FRAME
  FIXME: write documentation for enum definition
*/
/*!
  \var SoCamera::ViewportMapping SoCamera::ADJUST_CAMERA
  FIXME: write documentation for enum definition
*/
/*!
  \var SoCamera::ViewportMapping SoCamera::LEAVE_ALONE
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoCamera::viewportMapping
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoCamera::position
  FIXME: write documentation for field
*/
/*!
  \var SoSFRotation SoCamera::orientation
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoCamera::aspectRatio
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoCamera::nearDistance
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoCamera::farDistance
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoCamera::focalDistance
  FIXME: write documentation for field
*/


/*!
  \fn void SoCamera::scaleHeight(float scaleFactor)
  FIXME: write doc
*/

/*!
  \fn SbViewVolume SoCamera::getViewVolume(float useAspectRatio = 0.0f) const
  FIXME: write doc
*/

/*!
  \fn void SoCamera::viewBoundingBox(const SbBox3f & box, float aspect, float slack)
  FIXME: write doc
*/


SO_NODE_ABSTRACT_SOURCE(SoCamera);

/*!
  Constructor.
*/
SoCamera::SoCamera()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCamera);

  SO_NODE_ADD_FIELD(viewportMapping,(ADJUST_CAMERA));
  SO_NODE_ADD_FIELD(position,(0.0f, 0.0f, 1.0f));
  SO_NODE_ADD_FIELD(orientation,(SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(nearDistance,(1.0f));
  SO_NODE_ADD_FIELD(farDistance,(10.0f));
  SO_NODE_ADD_FIELD(aspectRatio,(1.0f));
  SO_NODE_ADD_FIELD(focalDistance,(5.0f));

  SO_NODE_DEFINE_ENUM_VALUE(ViewportMapping, CROP_VIEWPORT_FILL_FRAME);
  SO_NODE_DEFINE_ENUM_VALUE(ViewportMapping, CROP_VIEWPORT_LINE_FRAME);
  SO_NODE_DEFINE_ENUM_VALUE(ViewportMapping, CROP_VIEWPORT_NO_FRAME);
  SO_NODE_DEFINE_ENUM_VALUE(ViewportMapping, ADJUST_CAMERA);
  SO_NODE_DEFINE_ENUM_VALUE(ViewportMapping, LEAVE_ALONE);

  SO_NODE_SET_SF_ENUM_TYPE(viewportMapping, ViewportMapping);
}

/*!
  Destructor.
*/
SoCamera::~SoCamera()
{
}

/*!
  Does initialization common for all objects of the
  SoCamera class. This includes setting up the
  type system, among other things.
*/
void
SoCamera::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoCamera);

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoFocalDistanceElement);
  SO_ENABLE(SoGLRenderAction, SoGLProjectionMatrixElement);
  SO_ENABLE(SoGLRenderAction, SoViewVolumeElement);
  SO_ENABLE(SoGLRenderAction, SoGLViewingMatrixElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoFocalDistanceElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoProjectionMatrixElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoViewVolumeElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoViewingMatrixElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  SO_ENABLE(SoRayPickAction, SoFocalDistanceElement);
  SO_ENABLE(SoRayPickAction, SoProjectionMatrixElement);
  SO_ENABLE(SoRayPickAction, SoViewVolumeElement);
  SO_ENABLE(SoRayPickAction, SoViewingMatrixElement);
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  SO_ENABLE(SoCallbackAction, SoFocalDistanceElement);
  SO_ENABLE(SoCallbackAction, SoProjectionMatrixElement);
  SO_ENABLE(SoCallbackAction, SoViewVolumeElement);
  SO_ENABLE(SoCallbackAction, SoViewingMatrixElement);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

  SO_ENABLE(SoGetPrimitiveCountAction, SoFocalDistanceElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoProjectionMatrixElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoViewVolumeElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoViewingMatrixElement);
}

/*!
  FIXME: write function documentation
*/
void
SoCamera::pointAt(const SbVec3f & targetPoint)
{
  // FIXME: is this really correct? What if camera is not positioned
  // in or near origo and targetPoint is behind us, for instance?
  // 19990228 mortene.

  SbVec3f dir = targetPoint - position.getValue();
  SbRotation rot(SbVec3f(0.0f, 0.0f, -1.0f), dir);
  orientation.setValue(rot);
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoCamera::viewAll(SoNode * const sceneRoot,
                  const SbViewportRegion & vpRegion,
                  const float slack)
{
  SoGetBoundingBoxAction action(vpRegion);
  action.apply(sceneRoot);
  SbBox3f box = action.getBoundingBox();
#if 0 // debug
  SoDebugError::postInfo("SoCamera::viewAll",
                         "bbox: <%f %f %f>, <%f %f %f>\n",
                         box.getMin()[0], box.getMin()[1], box.getMin()[2],
                         box.getMax()[0], box.getMax()[1], box.getMax()[2]);
#endif // debug
  this->viewBoundingBox(box, aspectRatio.getValue(), slack);
}

/*!
  FIXME: write function documentation
*/
void
SoCamera::viewAll(SoPath * const path,
                  const SbViewportRegion &vpRegion,
                  const float slack)
{
  SoGetBoundingBoxAction action(vpRegion);
  action.apply(path);
  SbBox3f box = action.getBoundingBox();
  this->viewBoundingBox(box, aspectRatio.getValue(), slack);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

/*!
  FIXME: write function documentation
*/
SbViewportRegion
SoCamera::getViewportBounds(const SbViewportRegion & /* region */) const
{
  COIN_STUB();
  return SbViewportRegion();
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoCamera::GLRender(SoGLRenderAction *action)
{
  SoCamera::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoCamera::getBoundingBox(SoGetBoundingBoxAction *action)
{
  SoCamera::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write function documentation
*/
void
SoCamera::handleEvent(SoHandleEventAction * /* action */)
{
  // FIXME: anything to be done here? Perhaps something to get
  // SoHandleEventAction handling of ray picks correct? 19990214
  // mortene.
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

/*!
  FIXME: write function documentation
*/
void
SoCamera::jitter(int /* numPasses */, int /* curPass */,
                 const SbViewportRegion & /* vpReg */,
                 SbVec3f & /* jitterAmount */) const
{
  COIN_STUB();
}

#if !defined(COIN_EXCLUDE_DOACTION)
/*!
  FIXME: write doc
*/
void
SoCamera::doAction(SoAction *action)
{
  // FIXME: viewportMapping field is not accounted for. 19990315
  // mortene.
  SoState *state = action->getState();

  float aspectratio =
    SoViewportRegionElement::get(state).getViewportAspectRatio();

  SbViewVolume vv = this->getViewVolume(aspectratio);
  SoViewVolumeElement::set(state, this, vv);

  SbMatrix affine, proj;
  vv.getMatrices(affine, proj);
  affine.multRight(SoModelMatrixElement::get(state).inverse());
  SoProjectionMatrixElement::set(state, this, proj);
  SoViewingMatrixElement::set(state, this, affine);
  SoFocalDistanceElement::set(state, this, this->focalDistance.getValue());
}
#endif // !COIN_EXCLUDE_DOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
*/
void
SoCamera::callback(SoCallbackAction *action)
{
  SoCamera::doAction(action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
*/
void
SoCamera::rayPick(SoRayPickAction *action)
{
  SoCamera::doAction(action);
  action->computeWorldSpaceRay();
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoCamera::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoCamera::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
