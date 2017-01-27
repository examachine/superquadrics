//
//
// C++ Implementation for module: SQ_Window
//
// Description: 
//
//
// Author: exa
//
//

#include "SQ_Window.hxx"

SQ_Window::SQ_Window(void) : Gtk_Window(GTK_WINDOW_TOPLEVEL)
{
  set_title("SuperQuadrics Demonstration Program");
 
  // Create the OpenGL object 
  sq = new SQ_Display();
  sq->set_usize(360,360);
  sq->show();

  // Gtk_GLArea can't have a border, so we will stick it in a frame
  gl_frame = new Gtk_Frame();
  gl_frame->set_shadow_type (GTK_SHADOW_IN);
  gl_frame->set_border_width(8);  
  gl_frame->add(sq);   
  gl_frame->show();

  // Make 3 scrollbars to control the orientation
  adjx=new Gtk_Adjustment(0,0,360+30,5,30,30);
  scrollx = new Gtk_VScrollbar(*adjx);
  scrollx->set_usize(10,40);
  scrollx->show();

  adjy=new Gtk_Adjustment(0,0,360+30,5,30,30);
  scrolly = new Gtk_VScrollbar(*adjy);
  scrolly->set_usize(10,40);
  scrolly->show();

  adjz=new Gtk_Adjustment(0,0,360+30,5,30,30);
  scrollz = new Gtk_VScrollbar(*adjz);
  scrollz->set_usize(10,40);
  scrollz->show();

  // Make 2 scrollbars for epsilon1 and epsilon2
  adj_e1=new Gtk_Adjustment(1,0.2,5,0.1,1,1);
  scroll_e1 = new Gtk_HScrollbar(*adj_e1);
  scroll_e1->set_usize(120,10);
  scroll_e1->show();

  adj_e2=new Gtk_Adjustment(1,0.2,5,0.1,1,1);
  scroll_e2 = new Gtk_HScrollbar(*adj_e2);
  scroll_e2->set_usize(120,10);
  scroll_e2->show();

  // Make 3 scrollbars for a1,a2,a3
  adj_a1 =new Gtk_Adjustment(1,0.2,6,0.1,1,1);
  scroll_a1 = new Gtk_HScrollbar(*adj_a1);
  scroll_a1->set_usize(120,10);
  scroll_a1->show();

  adj_a2 =new Gtk_Adjustment(1,0.2,6,0.1,1,1);
  scroll_a2 = new Gtk_HScrollbar(*adj_a2);
  scroll_a2->set_usize(120,10);
  scroll_a2->show();

  adj_a3 =new Gtk_Adjustment(1,0.2,6,0.1,1,1);
  scroll_a3 = new Gtk_HScrollbar(*adj_a3);
  scroll_a3->set_usize(120,10);
  scroll_a3->show();

  // Create a Quit button
  quit_button = new Gtk_Button("Quit");
  quit_button->set_usize(80,40);
  quit_button->set_border_width(8);
  quit_button->show();

  // Superquadric types
  sq1_button = new Gtk_Button("Ellipsoid");
  sq1_button->set_usize(120,40);
  sq1_button->set_border_width(8);
  sq1_button->show();
  sq2_button = new Gtk_Button("Toroid");
  sq2_button->set_usize(120,40);
  sq2_button->set_border_width(8);
  sq2_button->show();
  sq3_button = new Gtk_Button("Hyperboloid");
  sq3_button->set_usize(120,40);
  sq3_button->set_border_width(8);
  sq3_button->show();
  sq4_button = new Gtk_Button("Hyperboloid-2");
  sq4_button->set_usize(120,40);
  sq4_button->set_border_width(8);
  sq4_button->show();

  // Place the rotation Scrollbars  in a Table
  rotation_box = new Gtk_Table(3,4,FALSE);
  rotation_box->attach(*scrollx,0,1,0,1,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  rotation_box->attach(*scrolly,1,2,0,1,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  rotation_box->attach(*scrollz,2,3,0,1,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  // Add some labels 
  labelx=new Gtk_Label("X");
  labelx->show();
  rotation_box->attach(*labelx,0,1,1,2,0,0,0,0);
  labely=new Gtk_Label("Y");
  labely->show();
  rotation_box->attach(*labely,1,2,1,2,0,0,0,0);
  labelz=new Gtk_Label("Z");
  labelz->show();
  rotation_box->attach(*labelz,2,4,1,2,0,0,0,0);
  //toolbox->attach(*quit_button,0,3,2,3,GTK_SHRINK,GTK_SHRINK,0,0);
  rotation_box->set_col_spacing(0,10);
  rotation_box->set_col_spacing(1,10);
  rotation_box->set_row_spacing(1,10);
  rotation_box->set_border_width(8);
  rotation_box->show();

  // Place the superquadrics controls
  superquadrics_box = new Gtk_Table(4,7,FALSE);
  superquadrics_box->attach(*sq1_button,0,1,0,1,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  superquadrics_box->attach(*sq2_button,0,1,1,2,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  superquadrics_box->attach(*sq3_button,0,1,2,3,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  superquadrics_box->attach(*sq4_button,0,1,3,4,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  superquadrics_box->attach(*scroll_e1,2,3,0,1,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  superquadrics_box->attach(*scroll_e2,2,3,1,2,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  superquadrics_box->attach(*scroll_a1,2,3,2,3,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  superquadrics_box->attach(*scroll_a2,2,3,3,4,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  superquadrics_box->attach(*scroll_a3,2,3,4,5,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  // Add some labels 
  labele1=new Gtk_Label("e1:");
  labele1->show();
  superquadrics_box->attach(*labele1,1,2,0,1,0,0,0,0);
  labele2=new Gtk_Label("e2:");
  labele2->show();
  superquadrics_box->attach(*labele2,1,2,1,2,0,0,0,0);
  labela1=new Gtk_Label("a1:");
  labela1->show();
  superquadrics_box->attach(*labela1,1,2,2,3,0,0,0,0);
  labela2=new Gtk_Label("a2:");
  labela2->show();
  superquadrics_box->attach(*labela2,1,2,3,4,0,0,0,0);
  labela3=new Gtk_Label("a3:");
  labela3->show();
  superquadrics_box->attach(*labela3,1,2,4,5,0,0,0,0);
  //toolbox->attach(*quit_button,0,3,2,3,GTK_SHRINK,GTK_SHRINK,0,0);
  superquadrics_box->set_col_spacing(0,8);
  superquadrics_box->set_col_spacing(1,8);
  superquadrics_box->set_row_spacing(1,8);
  superquadrics_box->set_border_width(8);
  superquadrics_box->show();

  // deform
  nodef = new Gtk_Button("No Deformation");
  nodef->set_usize(120,40);
  nodef->set_border_width(8);
  nodef->show();
  taperdef = new Gtk_Button("Tapering");
  taperdef->set_usize(120,40);
  taperdef->set_border_width(8);
  taperdef->show();
  twistdef = new Gtk_Button("Twisting");
  twistdef->set_usize(120,40);
  twistdef->set_border_width(8);
  twistdef->show();
  benddef = new Gtk_Button("Bending");
  benddef->set_usize(120,40);
  benddef->set_border_width(8);
  benddef->show();

  deformation_box = new Gtk_Table(1,5,FALSE);
  deformation_box->attach(*nodef,0,1,0,1,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  deformation_box->attach(*taperdef,0,1,1,2,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  deformation_box->attach(*twistdef,0,1,2,3,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  deformation_box->attach(*benddef,0,1,3,4,GTK_FILL,GTK_FILL|GTK_EXPAND|GTK_SHRINK,0,0);
  deformation_box->set_col_spacing(0,8);
  deformation_box->set_row_spacing(1,8);
  deformation_box->set_border_width(8);
  deformation_box->show();

  wireframe_but = new Gtk_Button("Wireframe");
  wireframe_but->set_usize(120,40);
  wireframe_but->set_border_width(8);
  wireframe_but->show();

  smooth_but = new Gtk_Button("Shaded");
  smooth_but->set_usize(120,40);
  smooth_but->set_border_width(8);
  smooth_but->show();

  rotation_frame = new Gtk_Frame();
  //rotation_frame->set_shadow_type (GTK_SHADOW_IN);
  rotation_frame->set_label("Rotation");
  rotation_frame->set_border_width(8);  
  rotation_frame->add(rotation_box);   
  rotation_frame->show();

  superquadrics_frame = new Gtk_Frame();
  //superquadrics_frame->set_shadow_type (GTK_SHADOW_IN);
  superquadrics_frame->set_label("Superquadrics Settings");
  superquadrics_frame->set_border_width(8);  
  superquadrics_frame->add(superquadrics_box);   
  superquadrics_frame->show();

  deformation_frame = new Gtk_Frame();
  //deformation_frame->set_shadow_type (GTK_SHADOW_IN);
  deformation_frame->set_label("Deformation Settings");
  deformation_frame->set_border_width(8);  
  deformation_frame->add(deformation_box);   
  deformation_frame->show();

  box3 = new Gtk_VBox(FALSE,0);
  box3->pack_start(*wireframe_but,false,false,0);
  box3->pack_start(*smooth_but,false,false,0);
  box3->show();

  rendering_frame = new Gtk_Frame();
  rendering_frame->set_label("Rendering Settings");
  rendering_frame->set_border_width(8);  
  rendering_frame->add(box3);   
  rendering_frame->show();

  box2 = new Gtk_HBox(FALSE,0);
  box2->pack_start(*deformation_frame,false,false,0);
  box2->pack_start(*rotation_frame,false,false,0);
  box2->pack_start(*rendering_frame,false,false,0);
  box2->show();
  box1 = new Gtk_VBox(FALSE,0);
  box1->pack_start(*superquadrics_frame,false,false,0);
  box1->pack_start(*box2,false,false,0);
  box1->show();

  // Pack the toolbar and OpenGL frame together
  box = new Gtk_HBox(FALSE,0);
  box->pack_start(*gl_frame,TRUE,TRUE,0);
  box->pack_start(*box1,TRUE,TRUE,0);
  //box->pack_start(*quit_button,FALSE,FALSE,0);
  box->show();

  // Add the box to the mainwindow
  add(box);

  // Connect the Quit button to the application quit
  connect_to_method(quit_button->clicked,Gtk_Main::instance(),&Gtk_Main::quit);

  // connect other buttons
  connect_to_method(sq1_button->clicked,sq,&SQ_Display::sq1);
  connect_to_method(sq2_button->clicked,sq,&SQ_Display::sq2);
  connect_to_method(sq3_button->clicked,sq,&SQ_Display::sq3);
  connect_to_method(sq4_button->clicked,sq,&SQ_Display::sq4);
  connect_to_method(nodef->clicked,sq,&SQ_Display::nodef);
  connect_to_method(taperdef->clicked,sq,&SQ_Display::taperdef);
  connect_to_method(twistdef->clicked,sq,&SQ_Display::twistdef);
  connect_to_method(benddef->clicked,sq,&SQ_Display::benddef);
  connect_to_method(wireframe_but->clicked,sq,&SQ_Display::wireframe_but);
  connect_to_method(smooth_but->clicked,sq,&SQ_Display::smooth_but);

  // Connect rotation scrollbars to the OpenGL object
  connect_to_method(adjx->value_changed,sq,&SQ_Display::setx,adjx);
  connect_to_method(adjy->value_changed,sq,&SQ_Display::sety,adjy);
  connect_to_method(adjz->value_changed,sq,&SQ_Display::setz,adjz);
  
  // Connect superquadric scroolbars to OpenGl obj.
  connect_to_method(adj_e1->value_changed,sq,&SQ_Display::sete1,adj_e1);
  connect_to_method(adj_e2->value_changed,sq,&SQ_Display::sete2,adj_e2);
  connect_to_method(adj_a1->value_changed,sq,&SQ_Display::seta1,adj_a1);
  connect_to_method(adj_a2->value_changed,sq,&SQ_Display::seta2,adj_a2);
  connect_to_method(adj_a3->value_changed,sq,&SQ_Display::seta3,adj_a3);

  // Connect the OpenGL object back to the scrollbars 
  connect_to_method(sq->updatex,(Gtk_Adjustment*)adjx,&Gtk_Adjustment::set_value);
  connect_to_method(sq->updatey,(Gtk_Adjustment*)adjy,&Gtk_Adjustment::set_value);
  connect_to_method(sq->updatez,(Gtk_Adjustment*)adjz,&Gtk_Adjustment::set_value);

  // Setup the idle function to make the globe rotate
  //connect_to_method(Gtk_Main::instance()->idle(),sq,&SQ_Display::idle);
}

SQ_Window::~SQ_Window(void)
{ 
  delete quit_button;
  delete box;
  delete gl_frame;
  delete labelx;
  delete labely;
  delete labelz;
  delete rotation_box;
  delete adjx;
  delete adjy;
  delete adjz;
  delete scrollx;
  delete scrolly;
  delete scrollz;
  delete sq;
};
