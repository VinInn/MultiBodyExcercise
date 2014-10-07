#ifndef NATIVEVEC_H
#define NATIVEVEC_H

#include<cmath>
#include<type_traits>
#include <x86intrin.h>
#include<cstring>


#ifdef __AVX__
#define NATIVE_VECT_LENGH 32
#else
#define NATIVE_VECT_LENGH 16
#endif


template<typename T>
struct NativeVectorTraits {
  typedef T __attribute__( ( vector_size(  NATIVE_VECT_LENGH ) ) ) type;
};


template<typename T> using NativeVector =  typename NativeVectorTraits<T>::type;


#ifdef __AVX__
  inline
  unsigned int mask(NativeVector<int> m) {
    return  _mm256_movemask_ps(NativeVector<float>(m));
  }
  inline
  unsigned int mask(NativeVector<float> m) {
    return  _mm256_movemask_ps(m);
  }
  inline
  unsigned int mask(NativeVector<long long> m) {
    return  _mm256_movemask_pd(NativeVector<double>(m));
  }
  inline
  unsigned int mask(NativeVector<double> m) {
    return  _mm256_movemask_pd(m);
  }
  inline
  NativeVector<float> sqrt(NativeVector<float> m) {
    return  _mm256_sqrt_ps(m);
  }
  inline
  NativeVector<double> sqrt(NativeVector<double> m) {
    return  _mm256_sqrt_pd(m);
  }
#else
  inline
  int mask(NativeVector<int> m) {
    return _mm_movemask_ps(NativeVector<float>(m));
  }
  inline
  int mask(NativeVector<float> m) {
    return _mm_movemask_ps(m);
  }
  inline
  unsigned int mask(NativeVector<long long> m) {
    return  _mm_movemask_pd(NativeVector<double>(m));
  }
  inline
  int mask(NativeVector<double> m) {
    return _mm_movemask_pd(m);
  }
  inline
  NativeVector<float> sqrt(NativeVector<float> m) {
    return  _mm_sqrt_ps(m);
  }
  inline
  NativeVector<double> sqrt(NativeVector<double> m) {
    return  _mm_sqrt_pd(m);
  }
#endif

namespace nativeVector {
  template<typename V1>
  inline
  constexpr unsigned int size(V1 a) { return sizeof(V1)/sizeof(a[0]);}

  template<typename V1>
    constexpr V1 zero() { return V1(NativeVector<int>()^NativeVector<int>());}


  
  template<typename V1,typename V2 >
  inline
  auto dot(V1 const & a, V2 const & b)  ->decltype(a[0]*b[0]) {
    constexpr unsigned int n = size(a);
    decltype(a[0]*b[0]) r=0;
    for (auto i=0U; i<n; ++i) r+=a[i]*b[i];
    return r;
  }
  
  template<typename V1>
  inline
  V1 min(V1 a, V1 b) {
    return (a<b) ? a : b;
  }
  template<typename V1>
  inline
  V1 max(V1 a, V1 b) {
    return (a>b) ? a : b;
  }
  
  template<typename V1>
  V1 abs(V1 a) {
    return (a>0) ? a : -a;
  }

  template<typename V1>
  inline
  bool any(V1 a) { return mask(a)!=0;}
  template<typename V1>
  inline
  bool all(V1 a) {
    constexpr unsigned int m = (0xFFFFFFFF)>>(32-sizeof(V1)/sizeof(decltype(a[0]))); 
    return mask(a)==m;
  }



  NativeVector<float> invSqrt(NativeVector<float> x){
    NativeVector<int> i;  memcpy(&i,&x,sizeof(i));
    i = 0x5f3759df - (i >> 1);
    NativeVector<float> y; memcpy(&y,&i,sizeof(i)); // very approximate
    y *= (1.5f - 0.5f * x * y * y); // better
    return y * (1.5f - 0.5f * x * y * y); // 2ULP
  }

}




#include<ostream>
  inline
  std::ostream & operator<<(std::ostream & co, NativeVector<float> const & v) {
     co << '('<< v[0];
     for (auto i=1U; i<  nativeVector::size(v); ++i)  co <<',' << v[i];
     return co <<')';
  }
  inline
  std::ostream & operator<<(std::ostream & co, NativeVector<double> const & v) {
     co << '('<< v[0];
     for (auto i=1U; i<  nativeVector::size(v); ++i)  co <<',' << v[i];
     return co <<')';
  }
  


#endif


