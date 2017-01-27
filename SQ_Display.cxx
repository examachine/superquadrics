//
//
// C++ Implementation for module: SQ_Display
//
// Description: 
//
//
// Author: exa
//
//

#include "SQ_Display.hxx"

SQ_Display::SQ_Display()
  : superquadric_mode(none), display_mode(gouraud), deformation_mode(no_def),
    mesh(NULL), deformed_mesh(NULL)
{
  xAngle=yAngle=zAngle=zRot=0;

  e1=e2=a1=a2=a3=1;

  //select events so that we can properly connect to them
  //GDK_POINTER_MOTION_HINT_MASK is used so that we 
  //don't get every point of motion
  set_events(GDK_EXPOSURE_MASK|
	     GDK_BUTTON_MOTION_MASK|
	     GDK_POINTER_MOTION_HINT_MASK|
	     GDK_BUTTON_PRESS_MASK|
	     GDK_BUTTON_RELEASE_MASK);

}

SQ_Display::~SQ_Display()
{
  // clear GL lists
  if (glIsList(test_list)==GL_TRUE)
    glDeleteLists(test_list,1);
}

void SQ_Display::realize_impl()
{
  Gtk_GLArea::realize_impl();
  initgl(); 
}

gint SQ_Display::motion_notify_event_impl(GdkEventMotion *event)
{
  gdouble x,y;
  gfloat xChange,yChange;
  GdkModifierType state;

  // Calculate the relative motion of the cursor
  if (event->is_hint)
    {gint tx,ty;
    gdk_window_get_pointer(event->window, &tx, &ty, &state);
    x=gdouble(tx);
    y=gdouble(ty);
    } 
  else 
    {x = event->x;
    y = event->y;
    state=GdkModifierType(event->state);
    }
  if (!(state&GDK_BUTTON1_MASK)) return TRUE;
  xChange=-0.25*(GLfloat)(basex-x);
  yChange=-0.25*(GLfloat)(basey-y);
  basex=event->x;
  basey=event->y;

  // Transform curser motions into object rotations
  yAngle+=cos(xAngle/180.0*M_PI)*xChange;
  zAngle-=sin(xAngle/180.0*M_PI)*xChange;
  xAngle+=yChange;
 
  // Limit the values of rotation
  while (xAngle<0)   xAngle+=360;
  while (xAngle>360) xAngle-=360;
  while (yAngle<0)   yAngle+=360;
  while (yAngle>360) yAngle-=360;
  while (zAngle<0)   zAngle+=360;
  while (zAngle>360) zAngle-=360;

  // Signal the scrollbars which the new values
  updatex(xAngle);
  updatey(yAngle);
  updatez(zAngle);

  // redraw the object
  drawgl();
  return TRUE;
}

gint SQ_Display::button_press_event_impl(GdkEventButton *event) 
{
  basex=event->x;
  basey=event->y;
  return TRUE;
}
gint SQ_Display::button_release_event_impl(GdkEventButton *event) 
{
  return TRUE;
}

gint SQ_Display::configure_event_impl(GdkEventConfigure *event)
{
  if (make_current()) {
    setupgl();
  }
  return TRUE;
}

gint SQ_Display::expose_event_impl(GdkEventExpose *event)
{
  // we are CPU intensive so we should wait for the last event
  if (event->count>0) return 0;
  drawgl();
  return TRUE;
}

double c1a,c1b,c2a,c2b;

double f1(double x) {
  return c1a+c1b*x;
};
double f1d(double x) {
  return c1b;
};

double f2(double x) {
  return c2a+c2b*x;
};
double f2d(double x) {
  return c2b;
};

// Create OpenGL lists
void SQ_Display::initgl()
{
  static GLfloat white[]={1.0,1.0,1.0,1.0};
  static GLfloat blue[]= {0.3,0.3,1.0,1.0};
  static GLfloat green[]={0.3,1.0,0.3,1.0};
  static GLfloat red[]=  {1.0,0.3,0.3,1.0};


  // In order to allow the window to be shrunk we will 
  // reset the minumum size to something smaller now 
  // that the inital allocation has been completed.
  set_usize(100,100);

  // make_current must be called before performing GL operations
  if (make_current()) {
    setupgl();

    if (superquadric_mode!=none) {
      if (mesh) delete mesh;
      switch (superquadric_mode) {
      case ellipsoid: mesh = new Ellipsoid_Mesh(26,26); break;
	
      case hyperboloid: mesh = new Hyperboloid_Mesh(26,26); break;
	
      case toroid: mesh = new Toroid_Mesh(26,26); break;
    
      case hyperboloid2: mesh = new Hyperboloid2_Mesh(26,26); break;
	// default:
	// error condition!!
      }
      mesh->gen_superquadric(e1,e2,a1,a2,a3);

      // cleanup
      if (deformed_mesh) {
	delete deformed_mesh;
	deformed_mesh=NULL;
      }
      if (deformation_mode!=no_def) {
	// transfer constants
	c1a = -1;
	c1b = 1/a3;
	c2a = -1;
	c2b =  1/a2;
      }

      switch (deformation_mode) {
      case taper_def: deformed_mesh = mesh->taper(f1,f1d); break;
      case twist_def: deformed_mesh = mesh->twist(f1,f1d); break;
      case bend_def: deformed_mesh = mesh->bend(-a2/2, a2/2, 0.7); break;
      case no_def: break;
      }
    }
  }
  //cerr << "opengl init" << endl;
}

// This function sets up the projection matrix and
// configures the OpenGL rendering engine. 
// This is called once to set up the initial matrix and
// then again if the window is resized.
void SQ_Display::setupgl()
{
  // Setup the rendering engine

  //lighting
    static GLfloat ambient[] = {0.1, 0.13, 0.24, 1.0};
  static GLfloat pos[4] = {28, 32, 73, 1 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv( GL_LIGHT0, GL_POSITION, pos );
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );

  // Polygon modes, etc.
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glClearColor(0.0, 0.0, 0.0, 0.0); // frame buffer clears should be to black
  //glEnable( GL_CULL_FACE );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_NORMALIZE );

  // material properties
  static GLfloat mat_diffuse[] = { 0.7, 0.4, 0.2, 1.0 };
  static GLfloat mat_specular[] = { 0.8, 0.7, 0.3, 1.0 };
  static GLfloat mat_shininess[] = { 64.0 };

  glClearColor (0.0, 0.0, 0.0, 1.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   
  glViewport(0, 0, (GLint)(width()), (GLint)(height()));

  // Create the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (width()>height()) 
    {
      GLfloat w = (GLfloat) width() / (GLfloat) height();
      glFrustum( -w, w, -1.0, 1.0, 2.0, 1000.0 );
    }
  else 
    {
      GLfloat h = (GLfloat) height() / (GLfloat) width();
      glFrustum( -1.0, 1.0, -h, h, 2.0, 1000.0 );
    }
  
  // Go to the place to start the rendering
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef( 0.0, 0.0, -12.0 );
}

// Calls the OpenGL list to display the
// object and then commits the rendering scene.
void SQ_Display::drawgl() 
{
  // start opengl rendering
  // all opengl calls must be placed inside make_current 
  if (make_current()) {
   
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();
    glRotatef( xAngle, 1.0, 0.0, 0.0 );
    glRotatef( yAngle, 0.0, 1.0, 0.0 );
    glRotatef( zAngle+zRot, 0.0, 0.0, 1.0 );

    glPushMatrix();
    if (superquadric_mode != none) {
      if (display_mode == gouraud) {
	active_mesh()->draw(true);
      } else
	active_mesh()->draw(false);
    }
    glPopMatrix();

    glPopMatrix();
  }
   
  swap_buffers();
  //cerr << "opengl draw" << endl;
}

// callbacks to setup rotations scrollbars and idle function
void SQ_Display::setx(Gtk_Adjustment *adj)
{
  xAngle=adj->get_value();
  drawgl();
}

void SQ_Display::sety(Gtk_Adjustment *adj)
{
  yAngle=adj->get_value();
  drawgl();
}
void SQ_Display::setz(Gtk_Adjustment *adj)
{
  zAngle=adj->get_value();
  drawgl();
}

void SQ_Display::sete1(Gtk_Adjustment *adj)
{
  e1=adj->get_value();
  initgl();
  drawgl();
}
void SQ_Display::sete2(Gtk_Adjustment *adj)
{
  e2=adj->get_value();
  initgl();
  drawgl();
}
void SQ_Display::seta1(Gtk_Adjustment *adj)
{
  a1=adj->get_value();
  initgl();
  drawgl();
}
void SQ_Display::seta2(Gtk_Adjustment *adj)
{
  a2=adj->get_value();
  initgl();
  drawgl();
}
void SQ_Display::seta3(Gtk_Adjustment *adj)
{
  a3=adj->get_value();
  initgl();
  drawgl();
}

void SQ_Display::sq1()
{
  superquadric_mode = ellipsoid;
  initgl();
  drawgl();
}
void SQ_Display::sq2()
{
  superquadric_mode = toroid;
  initgl();
  drawgl();
}
void SQ_Display::sq3()
{
  superquadric_mode = hyperboloid;
  initgl();
  drawgl();
}
void SQ_Display::sq4()
{
  superquadric_mode = hyperboloid2;
  initgl();
  drawgl();
}

void SQ_Display::nodef()
{
  deformation_mode = no_def;
  initgl();
  drawgl();
}
void SQ_Display::taperdef()
{
  deformation_mode = taper_def;
  initgl();
  drawgl();
}
void SQ_Display::twistdef()
{
  deformation_mode = twist_def;
  initgl();
  drawgl();
}
void SQ_Display::benddef()
{
  deformation_mode = bend_def;
  initgl();
  drawgl();
}
void SQ_Display::wireframe_but()
{
  display_mode = wireframe;
  initgl();
  drawgl();
}
void SQ_Display::smooth_but()
{
  display_mode = gouraud;
  initgl();
  drawgl();
}

int SQ_Display::idle()
{
  zRot+=0.03;
  if (zRot>360) zRot-=360;
  drawgl();
  return 1;  // we always want to be called so return 1
}



