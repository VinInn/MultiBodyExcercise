#ifndef EXTVEC_H
#define EXTVEC_H

#include<cmath>
#include<type_traits>
#include <x86intrin.h>


namespace extvec {

  template<typename T>
  struct IntType { using type = T; };
  template<>
  struct IntType<float> { using type = int; };
  template<>
  struct IntType<double> {  using type = long long;};

  template<>
  struct IntType<int> { using type = float; };
  template<>
  struct IntType<long long> {  using type = double;};
  
  
  template<typename TT, int N>
  struct ExtVecTraits {
    using T = typename std::remove_const<typename std::remove_reference<TT>::type>::type;
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
  




  inline
  float itof(int i) { float f;  memcpy(&f,&i,sizeof(float)); return f; }

  inline
  int ftoi(float f) { int i;  memcpy(&i,&f,sizeof(float)); return i;}


  
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
  
}

#include<ostream>
namespace extvec {
  inline
  std::ostream & operator<<(std::ostream & co, Vec4D<float> const & v) {
    return co << '('<< v[0] <<',' << v[1] <<',' <<  v[2] <<',' <<  v[3] <<')';  
  }
  inline
  std::ostream & operator<<(std::ostream & co, Vec4D<double> const & v) {
  return co << '('<< v[0] <<',' << v[1] <<',' <<  v[2] <<',' <<  v[3] <<')';  
  } 
}



#endif
