//
//
// C++ Implementation for module: SQ_GL
//
// Description: 
//
//
// Author: exa
//
//

#include "SQ_GL.hxx"


Mesh::Mesh(unsigned int w, unsigned int h) : width(w), height(h) {
  if (width<2 ||  height<2) {
    cerr << "mesh too small" << endl;
  }
  points.resize(width*height);
  normals.resize(width*height);
  hindex_lut.resize(height);
  int index_mul = 0;
  for (int i=0; i<height; i++, index_mul += width) {
    hindex_lut[i] = index_mul;
  }
}

void Mesh::draw(bool smooth) {

  // wireframe mode
  if (smooth) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
  }
  else {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glShadeModel(GL_FLAT);
    glDisable(GL_LIGHTING);
    glColor3f(0.05,0.64,0.09);
  }

  for (unsigned int j=0; j < height-1; j++) {
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3dv(get_normal(0,j));
    glVertex3dv(get_point(0,j));
    for (unsigned int i=0; i< width-1; i++) {
      glNormal3dv(get_normal(i,j+1));
      glVertex3dv(get_point(i,j+1));
      glNormal3dv(get_normal(i+1,j));
      glVertex3dv(get_point(i+1,j));
    }
    glNormal3dv(get_normal(width-1,j+1));
    glVertex3dv(get_point(width-1,j+1));
    glEnd();
  }
}


void Mesh::gen_superquadric(double epsilon1, double epsilon2,
			    double a1, double a2,double a3)
{
  // w == west and n == west in the original paper
  // ranges w: [-pi,pi] n: [-pi/2,pi/2]

  // generate an ellipsoid that will fill this mesh
  // we know exactly how many steps to take
  double w_start = -M_PI , n_start = -M_PI/2;
  double w_step = 2*M_PI / (width-1);
  double n_step = M_PI / (height-1);

  double w,n;
  n = n_start;
  for (int j = 0; j < height; j++, n += n_step) {
    w = w_start;
    for (int i = 0; i < width; i++, w += w_step)
      set_superquadric_point(i,j,w,n,epsilon1,epsilon2,a1,a2,a3);
  }
}

Mesh* Mesh::taper(double (*fx)(double),double (*fxd)(double)) {
  Mesh *m = new Mesh(width,height);
  for (int j = 0; j < height; j++)
    for (int i = 0; i < width; i++) {
      const Vector3& P = get_point(i,j);
      double r = fx(P.get_x());
      Vector3 Pn( r*P.get_x(), r*P.get_y(), P.get_z() );
      m->set_point(i, j, Pn);
      const Vector3& N = get_normal(i,j);
      Vector3 Nn( r*N.get_x(), r*N.get_y(),
		  -r*r*fxd(N.get_z())*N.get_x()*N.get_x() +
		  -r*r*fxd(N.get_z())*N.get_y()*N.get_y() + r*r*N.get_x());
      m->set_normal(i, j, Nn);
    }
  return m;
}

Mesh *Mesh::twist(double (*fz)(double),double (*fzd)(double)) {
  Mesh *m = new Mesh(width,height);
   for (int j = 0; j < height; j++)
    for (int i = 0; i < width; i++) {
      const Vector3& P = get_point(i,j);
      double theta = fz(P.get_z());
      double cos_theta = cos(theta), sin_theta = sin(theta);
      Vector3 Pn( P.get_x()*cos_theta - P.get_y()*sin_theta,
		  P.get_x()*sin_theta - P.get_y()*cos_theta,
		  P.get_z() );
      m->set_point(i, j, Pn);
      const Vector3& N = get_normal(i,j);
      Vector3 Nn( cos_theta*N.get_x() - sin_theta*N.get_y(),
		  sin_theta*N.get_x() + cos_theta*N.get_y(),
		  N.get_y()*fzd(N.get_z())*N.get_y() -
		  N.get_x()*fzd(N.get_z())*N.get_y() + N.get_z());
      m->set_normal(i, j, Nn);
    }
   return m;
}

Mesh *Mesh::bend(double ymin, double ymax,double k) {
  Mesh *m = new Mesh(width,height);
  double y_0 = ymin+ymax/2;
  for (int j = 0; j < height; j++)
    for (int i = 0; i < width; i++) {
      const Vector3& P = get_point(i,j);
      double theta,cos_theta,sin_theta;
      double x,y,z;
      if (y <= ymin) {
	theta = -k*(ymin - y_0);
	cos_theta = cos(theta), sin_theta = sin(theta);
	y = -sin_theta*(P.get_z()-1/k) + y_0;
	z = cos_theta*(P.get_z()-1/k)+1/k;
      }
      else if (y<ymax) {
	theta = -k*(y - y_0);
	cos_theta = cos(theta), sin_theta = sin(theta);
	y = -sin_theta*(P.get_z()-1/k) + y_0 + cos_theta*(P.get_y()-ymin);
	z = cos_theta*(P.get_z()-1/k)+1/k+sin_theta*(P.get_y()-ymin);
      }
      else {
	theta = -k*(ymax - y_0);
	cos_theta = cos(theta), sin_theta = sin(theta);
	y = -sin_theta*(P.get_z()-1/k) + y_0 + cos_theta*(P.get_y()-ymax);     
	z = cos_theta*(P.get_z()-1/k) + 1/k + sin_theta*(P.get_y()-ymax);
      }
      Vector3 Pn( P.get_x(), y, z );
      m->set_point(i, j, Pn);
      const Vector3& N = get_normal(i,j);
      if (y<ymin) {
	x = N.get_x();
	y = cos_theta*N.get_y() - sin_theta*N.get_z();
	z = sin_theta*N.get_y() + cos_theta*N.get_z();
      }
      else if (y<ymax) {
	x = (1-k*N.get_z())*N.get_x();
	y = cos_theta*N.get_y() - sin_theta*(1-k*N.get_z())*N.get_z();
	z = sin_theta*N.get_y() + cos_theta*(1-k*N.get_z())*N.get_z();
      }
      else {
	x = N.get_x();
	y = cos_theta*N.get_y() - sin_theta*N.get_z();
	z = sin_theta*N.get_y() + cos_theta*N.get_z();
      }
      Vector3 Nn(x,y,z);
      m->set_normal(i, j, Nn);
    }
  return m;
}


Ellipsoid_Mesh::Ellipsoid_Mesh(unsigned int w, unsigned int h)
  : Mesh(w,h)
{
  mesh_name="Ellipsoid";
}

void Ellipsoid_Mesh::set_superquadric_point(unsigned int i, unsigned int j,
					    double w, double n,
					    double epsilon1, double epsilon2,
					    double a1, double a2,double a3)
{
  set_point(i,j,Vector3( a1 * power(cos(n),epsilon1) * power(cos(w),epsilon2),
			 a2 * power(cos(n),epsilon1) * power(sin(w),epsilon2),
			 a3 * power(sin(n),epsilon1) ) );

  set_normal(i,j,Vector3( (1/a1) * power(cos(n),2-epsilon1) 
			  * power(cos(w), 2-epsilon2),
			  (1/a2) * power(cos(n),2-epsilon1)
			  * power(sin(w), 2-epsilon2),
			  (1/a3) * power(sin(n),2-epsilon1) ) );
}

Hyperboloid_Mesh::Hyperboloid_Mesh(unsigned int w, unsigned int h)
  : Mesh(w,h)
{
  mesh_name= "Hyperboloid";
}

void Hyperboloid_Mesh::set_superquadric_point(unsigned int i, unsigned int j,
					      double w, double n,
					      double epsilon1, double epsilon2,
					      double a1, double a2,double a3)
{
  set_point(i,j,Vector3( a1 * power(1/cos(n),epsilon1)
			 * power(cos(w),epsilon2),
			 a2 * power(1/cos(n),epsilon1)
			 * power(sin(w),epsilon2),
			 a3 * power(tan(n),epsilon1) ) );
  set_normal(i,j,Vector3( (1/a1) * power(1/cos(n),2-epsilon1) 
			  * power(cos(w), 2-epsilon2),
			  (1/a2) * power(1/cos(n),2-epsilon1)
			  * power(sin(w), 2-epsilon2),
			  (1/a3) * power(tan(n),2-epsilon1) ) );
}

Toroid_Mesh::Toroid_Mesh(unsigned int w, unsigned int h, double r)
  : Mesh(w,h), radius(r)
{
  mesh_name="Toroid";
}

void Toroid_Mesh::set_superquadric_point(unsigned int i, unsigned int j,
					      double w, double n,
					      double epsilon1, double epsilon2,
					      double a1, double a2,double a3)
{
  n *= 2;
  set_point(i,j,Vector3( a1 * (radius + power(cos(n),epsilon1))
			 * power(cos(w),epsilon2),
			 a2 * (radius + power(cos(n),epsilon1))
			 * power(sin(w),epsilon2),
			 a3 * power(sin(n),epsilon1) ) );
  set_normal(i,j,Vector3( (1/a1) * power(cos(n),2-epsilon1) 
			  * power(cos(w), 2-epsilon2),
			  (1/a2) * power(cos(n),2-epsilon1)
			  * power(sin(w), 2-epsilon2),
			  (1/a3) * power(sin(n),2-epsilon1) ) );
}

Hyperboloid2_Mesh::Hyperboloid2_Mesh(unsigned int w, unsigned int h, bool f)
  : Mesh(w,h), first(f)
{
  mesh_name="Hyperboloid - 2";
  if (first)
    second = new Hyperboloid2_Mesh(w,h,false);
}

void Hyperboloid2_Mesh::draw(bool smooth) {
  Mesh::draw(smooth);
  if (first) {
    second->draw(smooth);
  }
}

void Hyperboloid2_Mesh::gen_superquadric(double epsilon1, double epsilon2,
			    double a1, double a2,double a3)
{
  // w == west and n == west in the original paper
  // ranges w: [-pi/2,pi/2] n: [-pi/2,pi/2]

  // generate an ellipsoid that will fill this mesh
  // we know exactly how many steps to take
  double w_start;
  if (first)
    w_start =  -M_PI/2;
  else
    w_start = M_PI/2;
  double n_start = -M_PI/2;
  double w_step = M_PI / (width);
  double n_step = M_PI / (height);

  w_start += w_step/2;
  n_start += n_step/2;

  double w, n;
  n = n_start;
  for (int j = 0; j < height; j++, n += n_step) {
    w = w_start;
    for (int i = 0; i < width; i++, w += w_step)
      set_superquadric_point(i,j,w,n,epsilon1,epsilon2,a1,a2,a3);
  }
  if (first) {
    second->gen_superquadric(epsilon1,epsilon2,a1,a2,a3);
  }
 
}

void Hyperboloid2_Mesh::set_superquadric_point(unsigned int i, unsigned int j,
					      double w, double n,
					      double epsilon1, double epsilon2,
					      double a1, double a2,double a3)
{

  set_point(i,j,Vector3( a1 * power(1/cos(n),epsilon1)
			 * power(1/cos(w),epsilon2),
			 a2 * power(1/cos(n),epsilon1)
			 * power(tan(w),epsilon2),
			 a3 * power(tan(n),epsilon1) ) );
  set_normal(i,j,Vector3( (1/a1) * power(1/cos(n),2-epsilon1) 
			  * power(1/cos(w), 2-epsilon2),
			  (1/a2) * power(1/cos(n),2-epsilon1)
			  * power(tan(w), 2-epsilon2),
			  (1/a3) * power(tan(n),2-epsilon1) ) );
  
  Vector3 p = get_point(i,j);
  if (p.get_x() > 100)
    p.set_x( 100 );
  if (p.get_x() <- 100)
    p.set_x(-100);
  if (p.get_y() > 100)
    p.set_y(100);
  if (p.get_y() <- 100)
    p.set_y(-100);
  if (p.get_z() > 100)
    p.set_z(100);
  if (p.get_z() <- 100)
    p.set_z(-100);
  set_point(i,j,p);
  //cerr << "(" << p.get_x() << ", " << p.get_y() << ", " << p.get_z() << ")\n"; 

}






