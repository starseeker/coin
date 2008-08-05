#ifndef COIN_SBBASICP_H
#define COIN_SBBASICP_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/misc/SoBase.h>

template<typename To,typename From>
To
coin_internal_safe_cast(From * ptr) {
  To retVal;
  //We need to use retVal->getClassTypeId() syntax, as To is a
  //pointer, eg. To::getClassTypeId() will not work
  if((ptr != NULL) && ptr->isOfType(retVal->getClassTypeId()))
    return static_cast<To>(ptr);
  return NULL;
}
     
template<typename To>
To coin_safe_cast(const SoBase * ptr) { return coin_internal_safe_cast<To>(ptr); }
template<typename To>
To coin_safe_cast(SoBase * ptr) { return coin_internal_safe_cast<To>(ptr); }
template<typename To>
To coin_safe_cast(SoAction * ptr) { return coin_internal_safe_cast<To>(ptr); }


template<typename To,typename From>
To
coin_internal_assert_cast(From * ptr) {
  To retVal = coin_safe_cast<To>(ptr);
  assert(retVal && "ptr was not of correct type");
  return retVal;
}

template<typename To>
To coin_assert_cast(const SoBase * ptr) { return coin_internal_assert_cast<To>(ptr); }
template<typename To>
To coin_assert_cast(SoBase * ptr) { return coin_internal_assert_cast<To>(ptr); }
template<typename To>
To coin_assert_cast(SoAction * ptr) { return coin_internal_assert_cast<To>(ptr); }

//FIXME Should we remove this? - BFG 20080801
//Strictly for internal use, untill we know exactly how to handle these
template <typename To>
To 
reclassify_cast(SoPath * ptr) {
  return reinterpret_cast<To>(ptr);
}
template <typename To>
To 
reclassify_cast(const SoPath * ptr) {
  return reinterpret_cast<To>(ptr);
}
#endif // !COIN_SBBASICP_H