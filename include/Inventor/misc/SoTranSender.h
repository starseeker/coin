/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SOTRANSENDER_H
#define COIN_SOTRANSENDER_H

#include <Inventor/SbBasic.h>

class SoOutput;
class SoNode;


class COIN_DLL_EXPORT SoTranSender {

public:
  SoTranSender(SoOutput * output);
  ~SoTranSender();

  SoOutput * getOutput(void) const;
  void insert(SoNode * node);
  void insert(SoNode * node, SoNode * parent, int n);
  void remove(SoNode * parent, int n);
  void replace(SoNode * parent, int n, SoNode * newnode);
  void modify(SoNode * node);
  void prepareToSend(void);

private:
  // Placeholder for any data for the instance. Just added for the
  // sake of making it possible to implement this class after release
  // without breaking ABI compatibility.
  void * pimpl;
};

#endif // COIN_SOTRANSENDER_H
