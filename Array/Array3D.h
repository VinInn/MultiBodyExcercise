#ifndef Vector3D_H
#define Vector3D_H
#include<cmath>



template<typename T>
class Vector3D {
public:
  static constexpr T zero=0.; 
  static constexpr unsigned int DIM=4;
  using type = T;

  Vector3D(){}
  Vector3D(T x, T y, T z) : v{x,y,z,zero} {}
  // Vector3D(T x, T y, T z) : v{x,y,z} {}

  T x() const { return v[0];}
  T y() const { return v[1];}
  T z() const { return v[2];}

  T operator[](int i) const { return v[i];} 
  T & operator[](int i) { return v[i];} 


  Vector3D<T> operator-() {
    for (auto i=0U; i<Vector3D<T>::DIM; ++i) v[i]=-v[i];
    return *this;
  }


  Vector3D<T> operator+=(Vector3D<T>b) {
    for (auto i=0U; i<Vector3D<T>::DIM; ++i) v[i]+=b[i];
    return *this;
  }
  Vector3D<T> operator-=(Vector3D<T> b) {
    for (auto i=0U; i<Vector3D<T>::DIM; ++i) v[i]-=b[i];
    return *this;
  }

  Vector3D<T> operator*=(T b) {
    for (auto i=0U; i<Vector3D<T>::DIM; ++i) v[i]*=b;
    return *this;
  }

  Vector3D<T> operator/=(T b) {
    for (auto i=0U; i<Vector3D<T>::DIM; ++i) v[i]/=b;
    return *this;
  }


private:

  T v[DIM];

};

template<typename T>
Vector3D<T> operator+(Vector3D<T> a, Vector3D<T> b) {
  Vector3D<T> r = a; r+=b;
  return r;
}

template<typename T>
Vector3D<T> operator-(Vector3D<T> a, Vector3D<T> b) {
  Vector3D<T> r = a; r-=b;
  return r;
}

template<typename T>
Vector3D<T> operator*(T a, Vector3D<T> b) {
  Vector3D<T> r;
  for (auto i=0U; i<Vector3D<T>::DIM; ++i) r[i]=a*b[i];
  return r;
}

template<typename T>
Vector3D<T> operator*(Vector3D<T> b, T a) {
  Vector3D<T> r;
  for (auto i=0U; i<Vector3D<T>::DIM; ++i) r[i]=a*b[i];
  return r;
}

template<typename T>
Vector3D<T> operator/(Vector3D<T> b, T a) {
  Vector3D<T> r=b;
  for (auto i=0U; i<Vector3D<T>::DIM; ++i) r[i]/=a;
  return r;
}




template<typename T, int N>
struct ExtVecTraits {
  typedef T __attribute__( ( vector_size( N*sizeof(T) ) ) ) type;
};

template<typename T, int N> using ExtVec =  typename ExtVecTraits<T,N>::type;

template<typename T> using Vec4D = ExtVec<T,4>;





template<typename V1,typename V2 >
auto dot(V1 a, V2 b)  ->decltype(a[0]*b[0]) {
  decltype(a[0]*b[0]) r=0;
  for (auto i=0U; i<4; ++i) r+=a[i]*b[i];
  return r;
}



template<typename T>
T dot(Vector3D<T> a, Vector3D<T> b) {
  T r=0;
  for (auto i=0U; i<Vector3D<T>::DIM; ++i) r+=a[i]*b[i];
  return r;
}


template<typename V>
auto mag(V a) ->decltype(dot(a,a)) {
  return std::sqrt(dot(a,a));
} 


template<typename V1,typename V2 >
auto dist2(V1 a, V2 b) ->decltype(dot(a-b,a-b)){
  auto d = a-b;
  return dot(d,d);
}


template<typename V1,typename V2 >
auto dist(V1 a, V2 b) ->decltype(dist2(a,b)){
  return std::sqrt(dist2(a,b));
}


namespace vect3d {
  constexpr unsigned int DIM=4;
  using Float = float;

  constexpr float zerof=0.; 
  constexpr float zerod=0.; 
  struct ZERO{
    operator Vector3D<float>() const { return Vector3D<float>(zerof,zerof,zerof); }
    operator Vector3D<double>() const { return Vector3D<double>(zerod,zerod,zerod); }
    operator Vec4D<float>() const { return Vec4D<float>{zerof,zerof,zerof,zerof}; }
    operator Vec4D<double>() const { return Vec4D<double>{zerod,zerod,zerod,zerod}; }

  };

}




using Vect3D = Vector3D<vect3d::Float>;
//using Vect3D = Vec4D<vect3d::Float>;


#endif
