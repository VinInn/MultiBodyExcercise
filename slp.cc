#include "Vect.h"

#ifndef USEVECEXT
using Vec3D = Vector3D<float>;
#else
using Vec3D = extvec::Vec4D<float>;
#endif


Vec3D triad(Vec3D const & a, Vec3D const & b, Vec3D const & c) {
  return a+b*c;
}

Vec3D triadV(Vec3D a, Vec3D b, Vec3D c) {
  return a+b*c;
}


#ifndef USEVECEXT
Vec3D triad3(Vec3D const & a, Vec3D const & b, Vec3D const & c) {
  return store3(load3(a)+load3(b)*load3(c));
}
#endif
