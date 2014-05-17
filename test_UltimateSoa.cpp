#include "UltimateSoa.h"


template<typename T>
struct Vect {
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;
  using CREF = Vect<value const &>;
  using REF = Vect<ref>;
  using SOATUPLE = std::tuple<UltimateSoa<T,false>,UltimateSoa<T,false>,UltimateSoa<T,false>>;

  Vect() {}
  Vect(T ix, T iy, T iz, int k=0) : x(ix),y(iy),z(iz){}
  template<typename V>
  Vect(V const & v) : x(v.x), y(v.y), z(v.z) {}
  Vect(Vect const & v) : x(v.x), y(v.y), z(v.z) {}
  template<typename V>
  Vect& operator=(V const & v) { x=v.x; y=v.y; z=v.z; return *this; }
  Vect& operator=(Vect const & v) { x=v.x; y=v.y; z=v.z; return *this; }

  T x,y,z;
};


template<typename T>
struct Line {
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;
  using CREF = Line<value const &>;
  using REF = Line<ref>;
  using V = Vect<T>;
  using SOATUPLE = std::tuple<UltimateSoa<V,true>, UltimateSoa<V,true>>;

  Line() {}
  Line(V ip, V ic) : p(ip),c(ic){}
  template<typename L>
  Line(L const & l)  : p(l.p),c(l.c){}
  Line(Line const & l) : p(l.p),c(l.c){}
  template<typename L>
  Line& operator=(L const & l) { p=l.p; c=l.c;return *this; }
  Line& operator=(Line const & l){ p=l.p; c=l.c;return *this; }

  V p,c;
  
};





#include <iostream>
int main() {

  UltimateSoa<float,false>  f(10);

  UltimateSoa<Vect<float>,true> vv(10);

  UltimateSoa<Line<float>,true> vl(10);


  return (f[2]<0) & (vv[5].x>0) & (vl[3].p.y> vl[2].c.z);

};
