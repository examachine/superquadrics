//
//
// C++ Interface for module: SQ_Display
//
// Description: 
//
//
// Author: exa
//
//

#ifndef SQ_Display_Interface
#define SQ_Display_Interface

#include "General.hxx"
#include "SQ_GL.hxx"

class SQ_Display : public Gtk_GLArea
{
public:
  SQ_Display();
  ~SQ_Display();

  enum Display_Mode { gouraud, wireframe };
  enum Superquadric_Mode { none, ellipsoid, toroid, hyperboloid, hyperboloid2 };
  enum Deformation_Mode { no_def, taper_def,twist_def, bend_def};

  // callbacks we will connect to other widgets
  void setx(Gtk_Adjustment *);
  void sety(Gtk_Adjustment *);
  void setz(Gtk_Adjustment *);
  int idle();
  void sete1(Gtk_Adjustment *);
  void sete2(Gtk_Adjustment *);
  void seta1(Gtk_Adjustment *);
  void seta2(Gtk_Adjustment *);
  void seta3(Gtk_Adjustment *);
  void sq1();
  void sq2();
  void sq3();
  void sq4();
  void nodef();
  void taperdef();
  void twistdef();
  void benddef();
  void wireframe_but();
  void smooth_but();

  // signals to update scrollbars 
  Signal1<gfloat>updatex;
  Signal1<gfloat>updatey;
  Signal1<gfloat>updatez;

private:

  void initgl();
  void setupgl();
  void drawgl();

  // Mesh object
  Mesh *mesh, *deformed_mesh;

  // GL lists
  GLint test_list;

  // rotation
  GLfloat xAngle,yAngle,zAngle;
  GLfloat zRot;

  // for tracking mouse cursor
  gdouble basex,basey;

  Display_Mode display_mode;
  Superquadric_Mode superquadric_mode;
  Deformation_Mode deformation_mode; //is deformation applied?

  Mesh *active_mesh() {
    if (deformation_mode!=no_def)
      return deformed_mesh;
    else
      return mesh;
  }

  // superquadric parameters;
  double e1,e2,a1,a2,a3;

  // overrides
  virtual void realize_impl();
  virtual gint motion_notify_event_impl(GdkEventMotion*); 
  virtual gint button_press_event_impl(GdkEventButton*); 
  virtual gint button_release_event_impl(GdkEventButton*); 
  virtual gint expose_event_impl(GdkEventExpose* expose);
  virtual gint configure_event_impl(GdkEventConfigure *event);
  
};

#endif
