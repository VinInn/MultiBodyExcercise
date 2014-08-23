#ifndef Vect_H
#define Vect_H

#include "ExtVect.h"
#include "Vector3D.h"


#include "VecFun.h"

template<typename T>
extvec::Vec4D<T> load3(Vector3D<T> const & v) {
  using VA = typename extvec::ExtVecTraits<T,4>::typeA;
  return *(VA const *)(&v.x());
}

template<typename V >
auto store3(V ev) -> Vector3D<typename std::remove_const<typename std::remove_reference<decltype(ev[0])>::type>::type> {
  using T = typename std::remove_const<typename std::remove_reference<decltype(ev[0])>::type>::type;
  using VA = typename extvec::ExtVecTraits<T,4>::typeA;
  extvec::IVec4D<T> mask = {0,1,2,7};
  Vector3D<T> v; 
  decltype(auto) res = *(VA*)(&v.x());
  res = __builtin_shuffle(ev,res,mask);
  return v;
}


namespace vect3d {
  struct ZERO{
    operator Vector3D<float>() const { float zerol=0.; return Vector3D<float>(zerol,zerol,zerol); }
    operator Vector3D<double>() const { double zerol=0.; return Vector3D<double>(zerol,zerol,zerol); }
    operator extvec::Vec4D<float>() const { float zerol=0.; return extvec::Vec4D<float>{zerol,zerol,zerol,zerol}; }
    operator extvec::Vec4D<double>() const {  double zerol=0.; return extvec::Vec4D<double>{zerol,zerol,zerol,zerol}; }
  };
}

#endif
