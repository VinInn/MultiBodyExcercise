#include "SoaVector3D.h"

using V3D = Vector3D<float>;
using R3D = Vector3D<float&>;
using Soa = SOA3D<float>;

float weight(V3D v, Soa & s) {
  auto n = s.size();
  float a = 0.;
  for (auto i=0U; i<n; ++i) {
    a+= dist(v,s[i]);
  } 
  return a/float(n);
}

int main() {
  
  V3D v0 = vect3d::ZERO();
  float one = 1;
  V3D v2{one,one, one};

  float x[3]={1.f}, y[3]={1.f}, z[3]={1.f};
  R3D v1(x[0],y[0],z[0]);
  R3D k0(x[2],y[2],z[2]);
  
  
  k0 = v1+v2-v0;
  auto k1 = 7.f*v2;
  auto k2 = dot(k1,k0);
  auto k3 = dist(k0,k1)*k2;


  Soa s(128);auto n = s.size(); s[0]=k1;
  for (auto i=1U; i<n; ++i) s[i] = s[i-1]+k1;

    auto d = weight(k0,s);
  
  return d*k3>3.45f;



}
