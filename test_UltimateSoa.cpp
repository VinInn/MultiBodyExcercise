#include "UltimateSoa.h"


template<typename T>
struct Vect {
  using value = typename std::remove_const<typename std::remove_reference<T>::type>::type;
  using  ref = typename std::add_lvalue_reference<T>::type;
  using cref = typename std::add_lvalue_reference<typename std::add_const< T>::type>::type;
  using CREF = Vect<value const &>;
  using REF = Vect<ref>;
  using VAL = Vect<value>;
  using SOATUPLE = std::tuple<UltimateSoa<T>,UltimateSoa<T>,UltimateSoa<T>>;

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
  using V = Vect<T>;
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;

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


template<typename T>
struct UltimateSoaTraits<Line<T>> {
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;
  using CREF = Line<value const &>;
  using REF = Line<ref>;
  using VAL = Line<value>;
  using V = Vect<T>;
  using SOATUPLE = std::tuple<UltimateSoa<V>, UltimateSoa<V>>;
};




#include <iostream>
int main() {

  template<typename T> using Container = UltimateSoa<T>;

  Container<float>  f(10);

  Container<Vect<float>> vv(10);

  Container<Line<float>> vl(10);

  using V = Vect<float>;
  using L = Line<float>;

  for (auto i=0U; i<vl.size(); ++i) {
    auto val = float(i);
    vl[i] = L(V{val,val,val},V{val,val,val});
  }

  std::cout << vl[4].p.y << std::endl;
  std::cout << vl[7].p.y << std::endl;

  swap(vl[4],vl[7]);

  std::cout << vl[4].p.y << std::endl;
  std::cout << vl[7].p.y << std::endl;


  UltimateSoa<Line<float>>::swap(vl[4],vl[7]);

  std::cout << vl[4].p.y << std::endl;
  std::cout << vl[7].p.y << std::endl;


  return (f[2]<0) & (vv[5].x>0) & (vl[3].p.y> vl[2].c.z);

};
