//
//
// C++ Interface for module: SQ_Window
//
// Description: 
//
//
// Author: exa
//
//

#ifndef SQ_Window_Interface
#define SQ_Window_Interface

#include "General.hxx"
#include "SQ_Display.hxx"

class SQ_Window : public Gtk_Window 
{
public:
  SQ_Window();
  ~SQ_Window();
  gint delete_event_impl(GdkEventAny *) {
    Gtk_Main::instance()->quit();//CODE: get Gtk_App object
    return 0;
  }

private:
  // Pointers to all of the widgets
  Gtk_Button *quit_button,
    *sq1_button, *sq2_button, *sq3_button, *sq4_button,
    *nodef, *taperdef, *twistdef, *benddef,
    *wireframe_but, *smooth_but;
  Gtk_Box *box, *box1, *box2, *box3;
  Gtk_Frame *gl_frame,
    *rotation_frame,
    *superquadrics_frame,
    *deformation_frame,
    *rendering_frame;
  Gtk_Label *labelx,*labely,*labelz,
    *labele1,*labele2,*labela1,*labela2,*labela3;
  Gtk_Table *rotation_box,
    *superquadrics_box,
    *deformation_box;
  Gtk_Adjustment  *adjx,*adjy,*adjz,
    *adj_e1, *adj_e2,
    *adj_a1,*adj_a2,*adj_a3;
  Gtk_Scrollbar *scrollx,*scrolly,*scrollz,
    *scroll_e1, *scroll_e2,
    *scroll_a1,*scroll_a2,*scroll_a3;
  // OpenGLArea
  SQ_Display *sq;
};


#endif
