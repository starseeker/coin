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

/************************************************************************
 *
 * Generate a set of "valid" matrices (i.e. matrices put together from
 * a set of valid translations, scalevectors and rotations) on stdout.
 *
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/SbLinear.h>

float
rndf(void)
{
  return (float(rand()) / float(RAND_MAX) - 0.5f) * 4.0f;
}

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    (void)fprintf(stderr,
                  "\n\n\tUsage: %s NUM\n\n"
                  "\tNUM = number of matrices to output.\n\n",
                  argv[0]);
    exit(1);
  }

  SoDB::init();

  srand(19720408);
  int num = atoi(argv[1]);
  for (int i=0; i < num; i++) {
    SbMatrix m;
    m.setTransform(// translation
                   SbVec3f(rndf(), rndf(), rndf()),
                   // rotation
                   SbRotation(SbVec3f(rndf(), rndf(), rndf()), rndf()),
                   // scalevec
                   SbVec3f(rndf(), rndf(), rndf()),
                   // scaleorientation
                   SbRotation(SbVec3f(rndf(), rndf(), rndf()), rndf()),
                   // center
                   SbVec3f(rndf(), rndf(), rndf()));

    (void)fprintf(stdout,
                  "%.3f %.3f %.3f %.3f "
                  "%.3f %.3f %.3f %.3f "
                  "%.3f %.3f %.3f %.3f "
                  "%.3f %.3f %.3f %.3f\n",
                  m[0][0], m[0][1], m[0][2], m[0][3],
                  m[1][0], m[1][1], m[1][2], m[1][3],
                  m[2][0], m[2][1], m[2][2], m[2][3],
                  m[3][0], m[3][1], m[3][2], m[3][3]);
  }

  return 0;
}
