#include "Vector3D.h"

using Vec3D = Vector3D<float>;
//using Vec3D = Vec4D<float>;



Vec3D triad(Vec3D const & a, Vec3D const & b, Vec3D const & c) {
  return a+b*c;
}


