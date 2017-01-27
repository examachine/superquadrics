/*
**
** General Interface file
**
**
**
** exa
**
*/

#ifndef General_Interface
#define General_Interface

#include "ANSILibrary.hxx"

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <gtk--.h>
#include <gtkgl--/glarea.h>

inline
double power(double x, double y) {
  if (x < 0)
    return (-(pow(fabs(x), y)));
  else
    return(pow(x, y));
}

#endif



