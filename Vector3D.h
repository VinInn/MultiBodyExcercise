#ifndef Vector3D_H
#define Vector3D_H
#include<cmath>
#include<type_traits>
#include "align_allocator.h"
#include<vector>
#include <x86intrin.h>

template<typename T> using AVector = std::vector<T,align_allocator<T,32>>;


// #define USEDOUBLE

#define  USESOA

#ifndef USESOA
#define USEVECEXT
#endif


template<typename T>
class Vector3D {
public:
  static constexpr unsigned int DIM=3;
  using value = typename std::remove_const<typename std::remove_reference<T>::type>::type;
  using  ref = typename std::add_lvalue_reference<T>::type;
  using cref = typename std::add_lvalue_reference<typename std::add_const< T>::type>::type;
  using VAL = Vector3D<value>;
  using CREF = Vector3D<cref>;
  using REF = Vector3D<ref>;


  constexpr Vector3D(){}
  constexpr Vector3D(T ix, T iy, T iz) : xi(ix), yi(iy), zi(iz) {}
  template<typename V>
  constexpr Vector3D(V v) : xi(v.x()), yi(v.y()), zi(v.z()) {}
  template<typename V>
  Vector3D & operator=(V v) { xi = v.x();  yi=v.y(); zi=v.z(); return *this;}
  constexpr Vector3D(Vector3D const & v) : xi(v.x()), yi(v.y()), zi(v.z()) {}
  Vector3D & operator=(Vector3D const & v) { xi = v.x();  yi=v.y(); zi=v.z(); return *this;}
 
  


  cref x() const { return xi;}
  cref y() const { return yi;}
  cref z() const { return zi;}
  ref x() { return xi;}
  ref y() { return yi;}
  ref z() { return zi;}

#ifdef USESOA
  value operator[](unsigned int k) const {
    return (k==0) ? x() :( k==1 ? y() : z()); 
  }

  ref operator[](unsigned int k) {
    return (k==0) ? x() :( k==1 ? y() : z()); 
  }
#else
  cref operator[](unsigned int k) const {
    return *((&xi)+k);
  }

  ref operator[](unsigned int k) {
    return  *((&xi)+k);
  }
#endif


  template<typename V>
  Vector3D<T> & operator+=(V b) {
    xi+=b.x(); yi+=b.y(); zi+=b.z(); // wi+=b.wi;
    return *this;
  }

  template<typename V>
  Vector3D<T> & operator-=(V b) {
    xi-=b.x(); yi-=b.y(); zi-=b.z(); // wi-=b.wi;
    return *this;
  }

  template<typename V>
  Vector3D<T> & operator*=(V b) {
    xi*=b.x(); yi*=b.y(); zi*=b.z(); // wi*=b.wi;
    return *this;
  }

  template<typename V>
  Vector3D<T> & operator/=(V b) {
    xi/=b.x(); yi/=b.y(); zi/=b.z(); // wi*=b.wi;
    return *this;
  }


  Vector3D<T> & operator*=(value b) {
    xi*=b; yi*=b; zi*=b;  // wi*=b;
    return *this;
  }

  Vector3D<T> & operator/=(value b) {
    xi/=b; yi/=b; zi/=b; // wi/=b;
    return *this;
  }

private:

  T xi; T yi; T zi;
public:
  //  value wi=0;

};

template<typename T1, typename T2>
inline
Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type> 
operator+(Vector3D<T1> a, Vector3D<T2> b) {
  using V = Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>;
  //  return V(a.x()+b.x(),a.y()+b.y(),a.z()+b.z());
  V r=a; return r+=b;
}

template<typename T1, typename T2>
inline
Vector3D<typename std::remove_reference<T1>::type> 
operator-(Vector3D<T1> a, Vector3D<T2> b) {
  using V = Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>;
  //  return V(a.x()-b.x(),a.y()-b.y(),a.z()-b.z());
  V r=a; return r-=b;
}

template<typename T1, typename T2>
inline
Vector3D<typename std::remove_reference<T1>::type> 
operator*(Vector3D<T1> a, Vector3D<T2> b) {
  using V = Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>;
  V r=a; return r*=b;
}

template<typename T1, typename T2>
inline
Vector3D<typename std::remove_reference<T1>::type> 
operator/(Vector3D<T1> a, Vector3D<T2> b) {
  using V = Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>;
  V r=a; return r/=b;
}



template<typename T1, typename T2>
inline
Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>
operator*(T1 b, Vector3D<T2> a) {
  using V = Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>;
  V r=a; return r*=b;
  //  return V(a.x()*b,a.y()*b,a.z()*b);

}


template<typename T1, typename T2>
inline
Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>
operator*(Vector3D<T2> a, T1 b) {
  return b*a;
}


template<typename T1, typename T2>
inline
Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>
operator/(Vector3D<T2> a, T1 b) {
  Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type> r = a;
  return r/=b;
}



template<typename V1,typename V2 >
inline
auto dot(V1 const & a, V2 const & b)  ->decltype(a.x()*b.x()) {
  return a.x()*b.x() + a.y()*b.y() + a.z()*b.z();
}


template<typename T1, typename T2>
inline
Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>
max(Vector3D<T1> a, Vector3D<T2> b) {
  using V = Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>;
  return V(std::max(a.x(),b.x()),std::max(a.y(),b.y()),std::max(a.z(),b.z()));
}

template<typename T1>
inline
Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>
abs(Vector3D<T1> a) {
  using V = Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>;
  return V(std::abs(a.x()),std::abs(a.y()),std::abs(a.z()));
}



//-------------------------------------------

template<typename T>
struct IntType { using type = T; };
template<>
struct IntType<float> { using type = int; };
template<>
struct IntType<double> {  using type = long long;};


template<typename T, int N>
struct ExtVecTraits {
  typedef T __attribute__( ( vector_size( N*sizeof(T) ) ) ) type;
  typedef T __attribute__( ( vector_size( N*sizeof(T) ), aligned(alignof(T)) ) ) typeA;

  static type load(T const * p) { return *(typeA const *)(p);}
  static void load(T *p, type const & v) { *(typeA *)(p) = v; }

  static typeA & bind(T * p) { return *(typeA *)(p);}
  static typeA & bind(T const * p) { return *(typeA const *)(p);}

 typedef typename IntType<T>::type __attribute__( ( vector_size( N*sizeof(typename IntType<T>::type) ) ) ) itype;

};



template<typename T, int N> using ExtVec =  typename ExtVecTraits<T,N>::type;
template<typename T, int N> using ExtVecI =  typename ExtVecTraits<T,N>::itype;

template<typename T> using Vec4D = ExtVec<T,4>;
template<typename T> using IVec4D = ExtVecI<T,4>;



#ifdef USEVECEXT
template<typename V1,typename V2 >
auto dot(V1 const & a, V2 const & b)  ->decltype(a[0]*b[0]) {
  decltype(a[0]*b[0]) r=0;
  for (auto i=0U; i<4; ++i) r+=a[i]*b[i];
  return r;
}
 
template<typename V1>
V1 max(V1 a, V1 b) {
  return (a>b) ? a : b;
}

template<typename V1>
V1 abs(V1 a) {
  return (a>0) ? a : -a;
}

#ifdef __SSE2__
int mask(Vec4D<float> m) {
  return _mm_movemask_ps(m);
}
#endif

#ifdef __AVX2__
int mask(Vec4D<double> m) {
 return  _mm256_movemask_pd(m);
}
#elif __SSE2__
int mask(Vec4D<double> m) {
  typedef double __attribute__( ( vector_size( 2*sizeof(double) ) ) ) V2D;
  V2D & l = (V2D&)(m[0]);
  V2D & h = (V2D&)(m[2]);
  return _mm_movemask_pd(l) | (_mm_movemask_pd(h)<<2);
}
#endif

#endif




template<typename V>
inline
auto mag(V const & a) ->decltype(dot(a,a)) {
  return std::sqrt(dot(a,a));
} 


template<typename V1,typename V2 >
inline
auto dist2(V1 const & a, V2 const & b) ->decltype(dot(a-b,a-b)){
  auto && d = a-b;
  return dot(d,d);
  //auto pow2 = [](auto x) { return x*x;};
  //return pow2(a.x()-b.x()) + pow2(a.y()-b.y()) + pow2(a.z()-b.z());
}

template<typename V1,typename V2 >
inline
auto dist(V1 const & a, V2 const & b) ->decltype(dist2(a,b)){
  return std::sqrt(dist2(a,b));
}


namespace vect3d {
#ifdef  USEDOUBLE
  using Float = double;
#else
  using Float = float;
#endif

  constexpr float zerof=0.; 
  constexpr float zerod=0.; 
  struct ZERO{

    static Vector3D<Float> toV() { Float zerol=0.; return Vector3D<Float>(zerol,zerol,zerol); }

    operator Vector3D<float>() const { float zerol=0.; return Vector3D<float>(zerol,zerol,zerol); }
    operator Vector3D<double>() const { double zerol=0.; return Vector3D<double>(zerol,zerol,zerol); }
    operator Vec4D<float>() const { return Vec4D<float>{zerof,zerof,zerof,zerof}; }
    operator Vec4D<double>() const { return Vec4D<double>{zerod,zerod,zerod,zerod}; }

  };

}

#include<ostream>
template<typename T>
inline
std::ostream & operator<<(std::ostream & co, Vector3D<T> const & v) {
  return co << '('<< v.x() <<',' << v.y() <<',' <<  v.z() <<')';  
}
inline
std::ostream & operator<<(std::ostream & co, Vec4D<float> const & v) {
  return co << '('<< v[0] <<',' << v[1] <<',' <<  v[2] <<',' <<  v[3] <<')';  
}
inline
std::ostream & operator<<(std::ostream & co, Vec4D<double> const & v) {
  return co << '('<< v[0] <<',' << v[1] <<',' <<  v[2] <<',' <<  v[3] <<')';  
}


#include<vector>
template<typename T>
class SOA3D {
public:
  using V3D = Vector3D<T>;
  using R3D = Vector3D<T&>;
  using C3D = Vector3D<T const&>;

  SOA3D(){}
  explicit SOA3D(unsigned int is) : vx(is),vy(is),vz(is){}
  void resize(unsigned int n) { vx.resize(n);vy.resize(n);vz.resize(n);}  

  R3D operator[](unsigned int i) { return R3D(vx[i],vy[i],vz[i]); }
  C3D operator[](unsigned int i) const { return C3D(vx[i],vy[i],vz[i]); }
  
  unsigned int size() const { return vx.size();}
  
private:
  
  AVector<T> vx,vy,vz;
  
};


#endif
