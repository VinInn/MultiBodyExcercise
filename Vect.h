#include "ExtVect.h"
#include "Vector3D.h"


#ifdef USEVECEXT
using namespace extvec;
#endif


#include "VecFun.h"




namespace vect3d {
  struct ZERO{
    operator Vector3D<float>() const { float zerol=0.; return Vector3D<float>(zerol,zerol,zerol); }
    operator Vector3D<double>() const { double zerol=0.; return Vector3D<double>(zerol,zerol,zerol); }
    operator extvec::Vec4D<float>() const { float zerol=0.; return extvec::Vec4D<float>{zerol,zerol,zerol,zerol}; }
    operator extvec::Vec4D<double>() const {  double zerol=0.; return extvec::Vec4D<double>{zerol,zerol,zerol,zerol}; }
  };
}
