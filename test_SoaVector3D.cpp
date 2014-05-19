#include "Vector3D.h"
#include<cassert>

#include "rdtscp.h"

using V3D = Vector3D<float>;
using R3D = Vector3D<float&>;
using Soa = SOA3D<float>;

template<typename Cont>
float weight(V3D const & v, Cont const & s) {
  auto n = s.size();
  float a = 0.;
  for (auto i=0U; i<n; ++i) {
    a+= dist2(v,s[i]);
  } 
  return a/float(n);
}


template<typename Cont>
void update(V3D const & v, Cont & s) {
  auto n = s.size();
  for (auto i=0U; i<n; ++i) {
    s[i]+=v;
  } 
}



#include<iostream>
int go() {
  
  V3D v0 = vect3d::ZERO();
  float one = 1;
  V3D v2{one, 2, 3.14};

  float x[3]={1.f}, y[3]={1.f}, z[3]={1.f};
  R3D v1(x[0],y[0],z[0]);
  R3D k0(x[2],y[2],z[2]);
  
  
  k0 = v1+v2-v0;
  auto k1 = 7.f*v2;
  auto k2 = dot(k1,k0);
  auto k3 = dist(k0,k1)*k2;

  //std::vector<V3D> vv(1280);
  AVector<V3D> vv(1280);
  vv[0]=k1;
  auto nv = vv.size();
  for (auto i=1U; i<nv; ++i) vv[i] = vv[i-1]+k1;
  long long tv = -rdtscp();
  auto dv = weight(k0,vv);
  tv += rdtscp();

  Soa s(1280); auto n = s.size(); s[0]=k1;
  for (auto i=1U; i<n; ++i) s[i] = s[i-1]+k1;

  long long ts = -rdtscp();
  auto ds = weight(k0,s);
  ts += rdtscp();
 
  std::cout << dv << ' ' << ds << std::endl;
  std::cout << tv << ' ' << ts << std::endl;
  tv = -rdtscp();
  update(k0,vv);
  tv += rdtscp();
  ts = -rdtscp();
  update(k0,s);
  ts += rdtscp();
  std::cout << tv << ' ' << ts << std::endl;
  

  return ds*k3>3.45f;

}


int main() {
  int ret=0;
  for (int i=0; i<10; ++i)
    ret+=go();
  
  return ret;
}
