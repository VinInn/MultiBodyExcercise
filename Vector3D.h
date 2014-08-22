#ifndef Vector3D_H
#define Vector3D_H
#include<cmath>
#include<algorithm>
#include<type_traits>



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
  cref operator[](unsigned int k) const {
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
auto operator+(Vector3D<T1> a, Vector3D<T2> b) 
  -> Vector3D<typename std::remove_const<typename std::remove_reference<decltype(a.x()+b.x())>::type>::type> {
  using V = Vector3D<typename std::remove_const<typename std::remove_reference<decltype(a.x()+b.x())>::type>::type> ;
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



template<typename T1, typename T2>
inline
auto dot(Vector3D<T1> const & a, Vector3D<T2> const & b)  ->decltype(a.x()*b.x()) {
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



#include<ostream>
template<typename T>
inline
std::ostream & operator<<(std::ostream & co, Vector3D<T> const & v) {
  return co << '('<< v.x() <<',' << v.y() <<',' <<  v.z() <<')';  
}


#endif
