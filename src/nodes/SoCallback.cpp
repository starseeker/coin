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
  \class SoCallback SoCallback.h Inventor/nodes/SoCallback.h
  \brief The SoCallback class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoCallback.h>


#include <Inventor/actions/SoActions.h> // SoCallback uses all of them.

// *************************************************************************

SO_NODE_SOURCE(SoCallback);

/*!
  Constructor.
*/
SoCallback::SoCallback()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCallback);

  this->cbfunc = NULL;
}

/*!
  Destructor.
*/
SoCallback::~SoCallback()
{
}

/*!
  Does initialization common for all objects of the
  SoCallback class. This includes setting up the
  type system, among other things.
*/
void
SoCallback::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCallback);
}

/*!
  FIXME: write function documentation
*/
void
SoCallback::setCallback(SoCallbackCB * func, void * userdata)
{
  this->cbfunc = func;
  this->cbdata = userdata;
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void
SoCallback::doAction(SoAction * action)
{
  if (this->cbfunc) this->cbfunc(this->cbdata, action);
}
#endif // !COIN_EXCLUDE_SOACTION

// FIXME: seems a bit weird that they should all call only
// doAction()? 19990315 mortene.

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write function documentation
*/
void
SoCallback::callback(SoCallbackAction * action)
{
  SoCallback::doAction(action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoCallback::GLRender(SoGLRenderAction * action)
{
  SoCallback::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoCallback::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoCallback::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write function documentation
*/
void
SoCallback::getMatrix(SoGetMatrixAction * action)
{
  SoCallback::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write function documentation
*/
void
SoCallback::handleEvent(SoHandleEventAction * action)
{
  SoCallback::doAction(action);
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write function documentation
*/
void
SoCallback::pick(SoPickAction * action)
{
  SoCallback::doAction(action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
/*!
  FIXME: write function documentation
*/
void
SoCallback::search(SoSearchAction * action)
{
  SoCallback::doAction(action);
}
#endif // !COIN_EXCLUDE_SOSEARCHACTION

#if !defined(COIN_EXCLUDE_SOWRITEACTION)
/*!
  FIXME: write function documentation
*/
void
SoCallback::write(SoWriteAction * action)
{
  SoCallback::doAction(action);
}
#endif // !COIN_EXCLUDE_SOWRITEACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write function documentation
*/
void
SoCallback::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoCallback::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
*/
void
SoCallback::copyContents(const SoFieldContainer * /* fromFC */,
                         SbBool /* copyConnections */)
{
  COIN_STUB();
}
