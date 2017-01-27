//
//
// C++ Interface for module: SQ_GL
//
// Description: 
//
//
// Author: exa
//
//

#ifndef SQ_GL_Interface
#define SQ_GL_Interface

#include "General.hxx"

typedef double GL_Vector[3];

//typedef vect

class Vector3 {

public:

  Vector3() {}
  Vector3(double _x, double _y, double _z) {
    set_x(_x), set_y(_y), set_z(_z);
  }
  Vector3(double *vec) {
    rep[0] = vec[0];
    rep[1] = vec[1];
    rep[2] = vec[2];
  }

  operator const double* () const { return rep; }
  double *gen_gl_vector() {
    double *v = new double[3];
    v[0] = get_x(); v[1] = get_y(); v[2] = get_z();
    return v;
  }

  void operator=(GL_Vector& a) {
    a[0] = get_x(); a[1] = get_y(); a[2] = get_z(); 
  } 

  void gl_rotate() {
    glRotatef(get_x(),1.0,0.0,0.0);
    glRotatef(get_y(),0.0,1.0,0.0);
    glRotatef(get_z(),0.0,0.0,1.0);
  }

  void set_x(double val) { rep[0] = val; }
  void set_y(double val) { rep[1] = val; }
  void set_z(double val) { rep[2] = val; }

  double get_x() const { return rep[0]; }
  double get_y() const { return rep[1]; }
  double get_z() const { return rep[2]; }

  //double get_x() {
  //return rep[0];
  //}

  //private:
  //GL
private:
  GL_Vector rep;
};


inline bool operator ==(Vector3& a, Vector3& b) { return false; }
inline bool operator !=(Vector3& a, Vector3& b) { return false; }
inline bool operator <(Vector3& a, Vector3& b) { return false; }
inline bool operator >(Vector3& a, Vector3& b) { return false; }

class Mesh {
public:
  Mesh() {}

  Mesh(unsigned int w, unsigned int h);


  const string & get_mesh_name() const { return mesh_name; }

  unsigned int index (unsigned int x, unsigned int y) const {
    return x+hindex_lut[y];
  }

  Vector3& point_at(unsigned int x, unsigned int y) {
    return points[index(x,y)];
  }

  void set_point(unsigned int x, unsigned int y, const Vector3 & val) {
    points[index(x,y)] = val;
  }

  const Vector3 & get_point(unsigned int x, unsigned int y) const {
    return points[index(x,y)];
  }

  Vector3& normal_at(unsigned int x, unsigned int y) {
    return normals[index(x,y)];
  }

  void set_normal(unsigned int x, unsigned int y, const Vector3 & val) {
    normals[index(x,y)] = val;
  }

  const Vector3 & get_normal(unsigned int x, unsigned int y) const {
    return normals[index(x,y)];
  }

  // open gl draw
  virtual void draw(bool smooth=true); 

  // superquadric generation
  virtual void gen_superquadric(double epsilon1=1, double epsilon2=1.7,
				double a1=3, double a2=2,double a3=2);
  virtual void set_superquadric_point(unsigned int i, unsigned int j,
				      double w, double n,
				      double epsilon1, double epsilon2,
				      double a1,double a2,double a3)
    {} // pure virtual function is indeed better...

  // regular deformations, they take functions as arguments
  Mesh *taper(double (*fx)(double),double (*fxd)(double));
  Mesh *twist(double (*fz)(double),double (*fzd)(double));
  Mesh *bend(double ymin,double ymax,double k);

  string mesh_name;
  // member variables
  unsigned int width, height;
  vector<Vector3> points, normals; // positions vectors of points
  vector<unsigned int> hindex_lut;

};

class Ellipsoid_Mesh : public Mesh
{
public:
  Ellipsoid_Mesh(unsigned int w, unsigned int h);
  
  void set_superquadric_point(unsigned int i, unsigned int j,
			      double w, double n,
			      double epsilon1 , double epsilon2 ,
			      double a1 , double a2 ,double a3);
};

class Hyperboloid_Mesh : public Mesh
{
public:
  Hyperboloid_Mesh(unsigned int w, unsigned int h);
  
  void set_superquadric_point(unsigned int i, unsigned int j,
			      double w, double n,
			      double epsilon1 , double epsilon2 ,
			      double a1 , double a2 ,double a3);
};

class Toroid_Mesh : public Mesh
{
public:
  Toroid_Mesh(unsigned int w, unsigned int h, double r=1.5);
  
  void set_superquadric_point(unsigned int i, unsigned int j,
			      double w, double n,
			      double epsilon1 , double epsilon2 ,
			      double a1 , double a2 ,double a3);
private:
  double radius; // radius
};

class Hyperboloid2_Mesh : public Mesh
{
public:
  Hyperboloid2_Mesh(unsigned int w, unsigned int h, bool f=true);

  void draw(bool);
  
  void gen_superquadric(double epsilon1, double epsilon2,
			double a1, double a2,double a3);
  void set_superquadric_point(unsigned int i, unsigned int j,
			      double w, double n,
			      double epsilon1 , double epsilon2 ,
			      double a1 , double a2 ,double a3);
private:
  bool first;
  Hyperboloid2_Mesh* second;
};

#endif



