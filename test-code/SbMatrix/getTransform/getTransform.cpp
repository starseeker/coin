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
 * void SbMatrix::getTransform(SbVec3f &, SbRotation &, SbVec3f &,
 *                             SbRotation &) const
 *
 ************************************************************************/

#include <stdio.h>
#include <Inventor/SoDB.h>
#include <Inventor/SbLinear.h>


int
main(int argc, char ** argv)
{
  SoDB::init();

  // FIXME: istty(stdin). 20010114 mortene.

  int valsread;
  do {
    SbMat m;
    valsread = fscanf(stdin,
                      "%f %f %f %f "
                      "%f %f %f %f "
                      "%f %f %f %f "
                      "%f %f %f %f\n",
                      &m[0][0], &m[0][1], &m[0][2], &m[0][3],
                      &m[1][0], &m[1][1], &m[1][2], &m[1][3],
                      &m[2][0], &m[2][1], &m[2][2], &m[2][3],
                      &m[3][0], &m[3][1], &m[3][2], &m[3][3]);

    if (valsread == 16) {
      SbMatrix matrix(m);
      SbVec3f trans;
      SbRotation rot;
      SbVec3f scale;
      SbRotation so;
      matrix.getTransform(trans, rot, scale, so);

      (void)fprintf(stdout, "%.3f %.3f %.3f\n", trans[0], trans[1], trans[2]);
      const float * a = rot.getValue();
      (void)fprintf(stdout, "%.3f %.3f %.3f %3f\n", a[0], a[1], a[2], a[3]);
      (void)fprintf(stdout, "%.3f %.3f %.3f\n", scale[0], scale[1], scale[2]);
      a = so.getValue();
      (void)fprintf(stdout, "%.3f %.3f %.3f %3f\n", a[0], a[1], a[2], a[3]);
      (void)fprintf(stdout, "\n");
      
    }
  } while (valsread == 16);

  if (valsread != EOF) {
    (void)fflush(stdout);
    (void)fprintf(stderr, "Input data format error!\n");
  }

  return 0;
}
