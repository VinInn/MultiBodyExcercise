#ifndef UltimateSOA_H
#define UltimateSOA_H

#include<vector>
#include <tuple>
#include <utility>
#include <type_traits>


template<typename T, bool>
class UltimateSoa {};


template<typename T>
class UltimateSoa<T, true> {
public:
  using CREF = typename T::CREF;
  using REF =  typename T::REF;
  using Data = typename T::SOATUPLE;

   template<std::size_t... I>
   void resize_impl(unsigned int n, std::index_sequence<I...>) { 
     using swallow = int[];
     (void)swallow{0, ((void)(std::get<I>(data).resize(n)),0)...};
   }
   
  void resize(unsigned int n) {
    m_n = n;
    resize_impl(n,std::make_integer_sequence<std::size_t,std::tuple_size<Data>::value>{});
  }
  
  UltimateSoa(){}
   explicit UltimateSoa(unsigned int n) {
     resize(n);
   }

  auto size() const { return m_n;}

  template<typename V, std::size_t... I>
   V t2r_impl(int j, std::index_sequence<I...>) {
   return V(std::get<I>(data)[j] ...); 
 }

  REF operator[](int j) {
   return t2r_impl<REF>(j,std::make_integer_sequence<std::size_t,std::tuple_size<Data>::value>{});
 }

  CREF operator[](int j) const {
   return t2r_impl<CREF>(j,std::make_integer_sequence<std::size_t,std::tuple_size<Data>::value>{});
 }

  Data data;
  unsigned int m_n=0;
};


template<typename T>
class UltimateSoa<T, false> : public std::vector<T> {
public:
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;
  using VAL = UltimateSoa <value,false>;
  using CREF = UltimateSoa <value const &,false>;
  using REF = UltimateSoa<ref,false>;
  
  template<typename ... Args>
  UltimateSoa(Args... args) : std::vector<T>(args...){}

};


#endif





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
