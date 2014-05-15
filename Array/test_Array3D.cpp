#include "Array3D.h"


int main() {
  using V3D = Vector3D<float>;
  
  V3D v0 = vect3d::ZERO();
  V3D v1{1,1,1};
  V3D v2{1,1,1};
  
  auto k0 = v1+v2-v0;
  auto k1 = 7.f*v2;
  auto k2 = dot(k1,k0);
  auto k3 = dist(k0,k1)*k2;
  
  return k3>0;

}
