#ifndef SOAVector3D_H
#define SOAVector3D_H
#include<cmath>
#include<type_traits>



template<typename T>
class Vector3D {
public:
  static constexpr unsigned int DIM=3;
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;


  Vector3D(){}
  Vector3D(T ix, T iy, T iz) : xi(ix), yi(iy), zi(iz) {}
  template<typename V>
  Vector3D(V v) : xi(v.x()), yi(v.y()), zi(v.z()) {}
  // template<typename V>
  //Vector3D(V const & v) { xi = v.x();  yi=v.y(); zi=v.z();}
  //template<typename V>
  // Vector3D(V && v) : xi(v.x()), yi(v.y()), zi(v.z()) {}
  template<typename V>
  Vector3D & operator=(V const & v) { xi = v.x();  yi=v.y(); zi=v.z(); return *this;}



  value x() const { return xi;}
  value y() const { return yi;}
  value z() const { return zi;}
  ref x() { return xi;}
  ref y() { return yi;}
  ref z() { return zi;}

  value operator[](unsigned int k) const {
    return (k==0) ? x() :( k==1 ? y() : z()); 
  }

  ref operator[](unsigned int k) {
    return (k==0) ? x() :( k==1 ? y() : z()); 
  }


  template<typename V>
  Vector3D<T> operator-() {
    xi=-x(); yi=-y(); zi=-z();
    return *this;
  }


  template<typename V>
  Vector3D<T> operator+=(V b) {
    xi+=b.x(); yi+=b.y(); zi+=b.z();
    return *this;
  }

  template<typename V>
  Vector3D<T> operator-=(V b) {
    xi-=b.x(); yi-=b.y(); zi-=b.z();
    return *this;
  }

  Vector3D<T> operator*=(value b) {
    xi*=b; yi*=b; zi*=b;
    return *this;
  }

  Vector3D<T> operator/=(value b) {
    xi/=b; yi/=b; zi/=b;
    return *this;
  }

private:

  T xi; T yi; T zi;

};

template<typename T1, typename T2>
inline
Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type> operator+(Vector3D<T1> a, Vector3D<T2> b) {
  using V = Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>;
  return V(a.x()+b.x(),a.y()+b.y(),a.z()+b.z());
}

template<typename T1, typename T2>
inline
Vector3D<typename std::remove_reference<T1>::type> operator-(Vector3D<T1> a, Vector3D<T2> b) {
  using V = Vector3D<typename std::remove_const<typename std::remove_reference<T1>::type>::type>;
  return V(a.x()-b.x(),a.y()-b.y(),a.z()-b.z());
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





template<typename V>
inline
auto mag(V const & a) ->decltype(dot(a,a)) {
  return std::sqrt(dot(a,a));
} 


template<typename V1,typename V2 >
inline
auto dist2(V1 const & a, V2 const & b) ->decltype(dot(a-b,a-b)){
  //  auto && d = a-b;
  //return dot(d,d);
  auto pow2 = [](auto x) { return x*x;};
  return pow2(a.x()-b.x()) + pow2(a.y()-b.y()) + pow2(a.z()-b.z());

}

template<typename V1,typename V2 >
inline
auto dist(V1 const & a, V2 const & b) ->decltype(dist2(a,b)){
  return std::sqrt(dist2(a,b));
}


namespace vect3d {
  constexpr unsigned int DIM=4;
  using Float = float;

  constexpr float zerof=0.; 
  constexpr float zerod=0.; 
  struct ZERO{
    static Vector3D<float> toVF() { float zerol=0.; return Vector3D<float>(zerol,zerol,zerol); }
    static Vector3D<double> toVD() { double zerol=0.; return Vector3D<double>(zerol,zerol,zerol); }
    
    operator Vector3D<float>() const { float zerol=0.; return Vector3D<float>(zerol,zerol,zerol); }
    operator Vector3D<double>() const { double zerol=0.; return Vector3D<double>(zerol,zerol,zerol); }
    // operator Vec4D<float>() const { return Vec4D<float>{zerof,zerof,zerof,zerof}; }
    // operator Vec4D<double>() const { return Vec4D<double>{zerod,zerod,zerod,zerod}; }

  };

}


#include<vector>
template<typename T>
class SOA3D {
public:
  using V3D = Vector3D<T>;
  using R3D = Vector3D<T&>;
  using C3D = Vector3D<T const&>;


  explicit SOA3D(unsigned int is) : vx(is),vy(is),vz(is){}
  
  R3D operator[](unsigned int i) { return R3D(vx[i],vy[i],vz[i]); }
  C3D operator[](unsigned int i) const { return C3D(vx[i],vy[i],vz[i]); }
  
  unsigned int size() const { return vx.size();}
  
private:
  
  std::vector<T> vx,vy,vz;
  
};


using Vect3D = Vector3D<vect3d::Float>;
// using Vect3D = Vec4D<vect3d::Float>;


#endif
