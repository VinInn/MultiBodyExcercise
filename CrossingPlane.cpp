#include<cmath>
#include<cstdio>
#include<iostream>
#include <typeinfo>
\
// an "oriented" plane is defined by
// nx*(x-xp)+ny(y-yp)+nz*(z-zp)=0 (or in vector notation: n*(x-p) * here is the dot-product)
// for a generic point x,y,z this equation gives the distance of the point (with its sign) from the plane

// a line is represented by the "vector" equation x=xl+c*t (t is scalar and represent the path along the line from the "origin xl") 


template<typename T>
struct Vect {
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;

  Vect() {}
  Vect(T ix, T iy, T iz) : x(ix),y(iy),z(iz){}
  template<typename V>
  Vect(V v) : x(v.x), y(v.y), z(v.z) {}
  template<typename V>
  Vect& operator=(V v) { x=v.x; y=v.y; z=v.z; return *this; }

  T x,y,z;
  void normalize() {
    value in = value(1.)/std::sqrt(x*x+y*y+z*z);
    x*=in; y*=in; z*=in;
  }
};

template<typename T> using Point =  Vect<T>;

template<typename T1, typename T2>
inline
Vect<float> operator+(Vect<T1> const & a,Vect<T2> const & b) { return Vect<float>(a.x+b.x,a.y+b.y,a.z+b.z);}

template<typename T1, typename T2>
inline
Vect<float> operator-(Vect<T1> const & a,Vect<T2> const & b) { return Vect<float>(a.x-b.x,a.y-b.y,a.z-b.z);}

template<typename T>
inline
Vect<float> operator*(float s,Vect<T> const & a) { return Vect<float>(s*a.x,s*a.y,s*a.z);}

template<typename T>
inline
Vect<float> operator*(Vect<T> const & a,float s) { return Vect<float>(s*a.x,s*a.y,s*a.z);}

template<typename T1, typename T2>
inline
float dot(Vect<T1> const & a,Vect<T2> const & b) { return a.x*b.x+a.y*b.y+a.z*b.z;}

template<typename T1, typename T2>
inline
Vect<float> cross(Vect<T1> const & a,Vect<T2> const &b) {
  return Vect<float> (a.y*b.z-a.z*b.y,
		      a.z*b.x-a.x*b.z,
		      a.x*b.y-a.y*b.x
		      );
}


struct Surface {
   virtual ~Surface(){}
};

struct Trajectory {
  // virtual ~Trajectory(){}
};


template<typename T>
struct Plane : public Surface {
  Plane(): p(),n(){}
  Plane(Point<T> ip, Vect<T> in) : p(ip),n(in){}
  Point<T> p;
  Vect<T> n;
};

template<typename T>
struct Line final : public Trajectory {
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;
 
 Line(): p(),c(){}
 Line(Point<T> const & ip, Vect<T> const & ic) : p(ip),c(ic){}
 template<typename L>
   Line(L l) : p(l.p),c(l.p){}
 template<typename L>
   Line& operator=(L l) {p=l.p; c=l.p; return *this;}
  Point<T> p;
  Vect<T> c;

  Point<value> go(float t) const { return p + c*t; }
};



// return t for the point of plane-line crossing
template<typename T1, typename T2>
inline
float cross(Plane<T1> const & plane, Line<T2> const & line) {
  return dot(plane.n,plane.p-line.p)/dot(plane.n,line.c);
}

// return distance of a point from a plane
template<typename T1, typename T2>
inline
float distance(Plane<T1> const & plane, Point<T2> const & point) {
  return dot(plane.n,point-plane.p);
}


template<typename T>
struct Quadrangle : public Plane<T> {
  Quadrangle() {}
  Quadrangle(Point<T> const & ip, Vect<T> const & iu, Vect<T> const & iv, T iul, T ivl) : 
    Plane<T>(ip,cross(iu,iv)),u(iu),v(iv), ul(iul), vl(ivl){}
  Vect<T> u, v;
  T ul, vl;
};

template<typename T>
struct Rectangle final : public Quadrangle<T> {
  template<typename ...Args>
  Rectangle(Args &&...args) :  Quadrangle<T>(std::forward<Args>(args)...){}
};


template<typename T>
struct BaseTrapezoid :  public Quadrangle<T> {
  BaseTrapezoid(){}
  BaseTrapezoid(Point<T> const & ip, Vect<T> const & iu, Vect<T> const & iv, T ius, T iul, T ivl) : 
    Quadrangle<T>(ip,iu,iv, 0.5f*(ius+iul), ivl), alpha((iul-ius)/(ius+iul)) {}
  T alpha;
};

template<typename T>
struct Trapezoid final : public BaseTrapezoid<T> {
  template<typename ...Args>
  Trapezoid(Args &&...args) :  BaseTrapezoid<T>(std::forward<Args>(args)...){}
};


// p is not the baricenter: is on the "straight" vertical side
template<typename T>
struct LeftTrapezoid final : public BaseTrapezoid<T> {
  template<typename ...Args>
  LeftTrapezoid(Args &&...args) :  BaseTrapezoid<T>(std::forward<Args>(args)...){}
};

// p is not the baricenter: is on the "straight" vertical side
template<typename T>
struct RightTrapezoid final : public BaseTrapezoid<T> {
  template<typename ...Args>
  RightTrapezoid(Args &&...args) :  BaseTrapezoid<T>(std::forward<Args>(args)...){}
};

template<typename T1, typename T2>
inline 
void locCor(Quadrangle<T1> const & r, Point<T2> const & p,float & u,float & v) {
  Vect<float> d = p-r.p;
  // printf("d= %e,%e,%e\n",d.x,d.y,d.z);
  u = dot(d,r.u);
  v = dot(d,r.v);
  // printf("u,v= %e,%e\n",u,v);
}

template<typename T1, typename T2>
inline
bool inside(Rectangle<T1> const & r, Point<T2> const & p) {
  float u,v;
  locCor(r,p,u,v);
  return  (r.ul>std::abs(u) ) &  ( r.vl>std::abs(v) ) ;
}


template<typename T1, typename T2>
inline
bool inside(Trapezoid<T1> const & r, Point<T2> const & p) {
  float u,v;
  locCor(r,p,u,v);
  float ul = r.ul + v * r.alpha;
  // printf("u,v= %e,%e\n",u,v);
  return  (ul>u) &  ( r.vl>std::abs(v) ) ;
}

template<typename T1, typename T2>
inline
bool inside(LeftTrapezoid<T1> const & r, Point<T2> const & p) {
  float u,v;
  locCor(r,p,u,v);
  float ul = r.ul + v * r.alpha;
  // printf("u,v= %e,%e\n",u,v);
  return  (v>0.f ) & (ul>u  ) &  ( (-r.vl)<v ) ;
}

template<typename T1, typename T2>
inline
bool inside(RightTrapezoid<T1> const & r, Point<T2> const & p) {
  float u,v;
  locCor(r,p,u,v);
  float ul = r.ul + v * r.alpha;
  // printf("u,v= %e,%e\n",u,v);
  return  (u<0.f) & (ul>u ) &  ( r.vl>v ) ;
}





// SOA
struct Vects {
  static constexpr int nValues=3;
  using VV = Vect<float>;
  using RV = Vect<float&>;
  using CV = Vect<float const & >;

  Vects() : mem(nullptr),m_size(0){}
  Vects(int s, float * imem) : mem(imem),m_size(s){}
  float * __restrict__ mem;
  int m_size;
  
  float & x(int i) { return mem[i];}
  float & y(int i) { return mem[i+size()];}
  float & z(int i) { return mem[i+size()+size()];}

  float const & x(int i) const { return mem[i];}
  float const & y(int i) const { return mem[i+size()];}
  float const & z(int i) const { return mem[i+size()+size()];}


  CV operator[](int i) const {
    return CV(x(i),y(i),z(i));
  } 
  RV operator[](int i) {
    return RV(x(i),y(i),z(i));
  } 

  int size() const { return m_size;}
  

  
};



using Points =  Vects;

struct Lines {
  static constexpr int nValues=2*Vects::nValues;

  using VL = Line<float>;
  using RL = Line<float&>;
  using CL = Line<float const & >;


  Lines(){}
  Lines(int s, float * mem):p(s,mem),c(s,mem+s*Vects::nValues){}
  Vects p, c;

  CL operator[](int i) const { return CL(p[i],c[i]);}
  RL operator[](int i) { return RL(p[i],c[i]);}
  int size() const { return p.size();  }

};

inline
Plane<float> makePlane(float offr=0) {
  // a generic plane
  double nx = -7./13., ny=3./7., nz=17./23.; 
  double nor = 1./std::sqrt(nx*nx+ny*ny+nz*nz); 
  return Plane<float>({float(3./7.+offr),float(4./7.+offr),13./17.},{float(nx*nor),float(ny*nor),float(nz*nor)});
}

inline
Rectangle<float>  makeRectangle(float offr=0) {
  // a generic Rectangle
  double nx = -7./13., ny=3./7., nz=17./23.; 
  double nor = 1./sqrt(nx*nx+ny*ny+nz*nz);
  Vect<float>  n = {float(nx*nor),float(ny*nor),float(nz*nor)};
  nor = 1./sqrt(nx*nx+ny*ny);
  Vect<float>  u = {float(ny*nor),float(-nx*nor),0};
  Vect<float>  v = cross(n,u);
  Point<float>  p = {float(3./7.+offr),float(4./7.+offr),13./17.};
  return Rectangle<float> (p,u,v,27./17.,15./17.);
}

inline
LeftTrapezoid<float>  makeLeftTrapezoid(float offr=0) {
  // a generic Rectangle
  double nx = -7./13., ny=3./7., nz=17./23.; 
  double nor = 1./sqrt(nx*nx+ny*ny+nz*nz);
  Vect<float> n = {float(nx*nor),float(ny*nor),float(nz*nor)};
  nor = 1./sqrt(nx*nx+ny*ny);
  Vect<float>  u = {float(ny*nor),float(-nx*nor),0};
  Vect<float>  v = cross(n,u);
  Point<float>  p = {float(3./7.+offr),float(4./7.+offr),13./17.};
  return LeftTrapezoid<float> (p,u,v,21./17.,27./17., 15./17.);
}


inline
Line<float>  makeLine(float r, float z) {
  // a generic line (almost parallel to the plane...)
  double cy = -7./13.+r, cx=3./7.-r, cz=17./23.+z; 
  double nor = 1./sqrt(cx*cx+cy*cy+cz*cz); 
  return Line<float> ({3.3/7.,4.4/7.,13./17.},{float(cx*nor),float(cy*nor),float(cz*nor)});
}

inline
void makeLines(Lines & lines) {
  float epsr = 0.5/13., epsz=1./23.;
  float r=0, z=0;
  int n = lines.size();
  for (int i=0; i!=n; ++i) {
    r+=epsr;
    z += (i<n/2) ? epsz : -epsz;
    lines[i] = makeLine(r,z);
  }
}



void testCross() {
  Plane<float>  pl = makePlane();
  Line<float>   ln = makeLine(0,0);

  float d0 = distance(pl,ln.p);

  printf("d0 = %e, %a\n",d0,d0);
  float t = cross(pl,ln);
  printf("t  = %e, %a\n",t,t);
  Point<float>  x1 = ln.go(t);
  float d1 = distance(pl,x1);
  printf("d1 = %e, %a\n",d1,d1);
  t = nextafterf(t,copysignf(999999.f,t));
  printf("t+ = %e, %a\n",t,t);
  Point<float>  x2 = ln.go(t);
  float d2 = distance(pl,x2);
  printf("d2 = %e, %a\n",d2,d2);
  printf("\n");

}

void testInside() {
  Rectangle<float>  r = makeRectangle();
  bool ic = inside(r,r.p+0.9*r.ul*r.u-0.5*r.vl*r.v);
  std::cout << (ic ? "inside" : "outside") << std::endl;
  ic = inside(r,r.p+1.2*r.ul*r.u-0.5*r.vl*r.v);
  std::cout << (ic ? "inside" : "outside") << std::endl;
  printf("\n");
}


void loopCross() {
  int N=256;
  float arena[N*Lines::nValues];
  Lines lines(N,arena);
  makeLines(lines);

  auto pl = makePlane();
  int n = lines.size();
  float d2[n];
  for (int i=0; i!=n; ++i) {
    float t = cross(pl,lines[i]);
    auto  x2 = lines[i].go(t);
    d2[i] = distance(pl,x2);
  }

  printf("\n");
  for (int i=0; i!=n; ++i)
    printf("%e,%a ",d2[i],d2[i]);
  printf("\n");
}



template<typename S>
inline
void loopInsideKernel(S const & r) {
  int N=256;
  float arena[N*Lines::nValues];
  Lines lines(N,arena);
  makeLines(lines);

  int n = lines.size();
  bool in[n];
  for (int i=0; i<n; ++i) {
    float t = cross(r,lines[i]);
    auto  x2 = lines[i].go(t);
    //bool ok = x2.x >0;
    bool ok = inside(r,x2);
    auto && xx = ok ? x2 : decltype(x2)(lines[i].p);
    // if (in[i]) 
    lines[i].p  = xx;
  }

  printf("\n");
  std::cout << typeid(S).name() << std::endl;
  for (int i=0; i!=n; ++i)
    std::cout << ( in[i] ? 'i' : 'o');
  std::cout << std::endl;
  for (int i=0; i!=n; ++i)
    std::cout <<  lines[i].p.x <<",";
  std::cout << std::endl;
}


void loopInsideR() {
  auto r = makeRectangle(0.01f);
  loopInsideKernel(r);
}

void loopInsideLT() {
  auto r = makeLeftTrapezoid(0.01f);
  loopInsideKernel(r);
}

int main() {
  testCross();
  testInside();

  loopCross();
  loopInsideR();

  loopInsideLT();
}
