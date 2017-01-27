//
//
// C++ Implementation file for application
//
// Description: includes the main(...) function
//
// exa
//
//

#include "General.hxx"
#include "SQ_Window.hxx"

void main(int argc, char *argv[])
{
  Gtk_Main app(&argc,&argv);

  SQ_Window* sq_window = new SQ_Window;
  sq_window->show();

  app.run();

  return 0;
}
