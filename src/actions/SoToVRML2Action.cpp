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
  \class SoToVRML2Action Inventor/include/SoToVRML2Action.h
  \brief The SoToVRML2Action class builds a new scene graph, using only
  VRML 2.0 nodes.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoToVRML2Action.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource(SoToVRML2Action)

SoType SoToVRML2Action::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoToVRML2Action::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoToVRML2Action::getTypeId(void) const
{
  return classTypeId;
}

#include <assert.h>

// static variables
SoEnabledElementsList * SoToVRML2Action::enabledElements;
SoActionMethodList * SoToVRML2Action::methods;

/*!
  \fn SoToVRML2Action::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoToVRML2Action::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoToVRML2Action::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void
SoToVRML2Action::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void
SoToVRML2Action::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes static data for the
  SoToVRML2Action class.
*/

void
SoToVRML2Action::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource(SoToVRML2Action)
  assert(SoToVRML2Action::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoToVRML2Action::classTypeId =
      SoType::createType(inherited::getClassTypeId(),
                         "SoToVRML2Action");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

// *************************************************************************

/*!
  A constructor.
*/

SoToVRML2Action::SoToVRML2Action(void)
{
  COIN_STUB();
}

/*!
  The destructor.
*/

SoToVRML2Action::~SoToVRML2Action(void)
{
  // FIXME: implement.
}

/*!
  This method is called before the scene graph traversal is begun.
*/

void
SoToVRML2Action::beginTraversal(SoNode * /* node */)
{
  COIN_STUB();
}
