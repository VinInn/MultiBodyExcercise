#ifdef USEVECEXT
#include "ExtVec.h"
#else
#include "Vector.h"
#endif


#ifdef USEVECEXT
using namespace extvec;
#endif


#include "VecFun.h"




namespace vect3d {
  struct ZERO{
   operator Vector3D<float>() const { float zerol=0.; return Vector3D<float>(zerol,zerol,zerol); }
    operator Vector3D<double>() const { double zerol=0.; return Vector3D<double>(zerol,zerol,zerol); }
    operator extvec::Vec4D<float>() const { return extvec::Vec4D<float>{zerof,zerof,zerof,zerof}; }
    operator extvec::Vec4D<double>() const { return extvec::Vec4D<double>{zerod,zerod,zerod,zerod}; }
  };
}
