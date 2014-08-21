#ifndef SoaV3D_H
#define SoaV3D_H

#include "Vector3d.h"

#include "align_allocator.h"
#include<vector>


// #define USEDOUBLE

// #define  USESOA

#ifndef USESOA
// #define USEVECEXT
#endif




template<typename T> using AVector = std::vector<T,align_allocator<T,32>>;


template<typename T>
class SOA3D {
public:
  using V3D = Vector3D<T>;
  using R3D = Vector3D<T&>;
  using C3D = Vector3D<T const&>;

  SOA3D(){}
  explicit SOA3D(unsigned int is) : vx(is),vy(is),vz(is){}
  void resize(unsigned int n) { vx.resize(n);vy.resize(n);vz.resize(n);}  

  R3D operator[](unsigned int i) { return R3D(vx[i],vy[i],vz[i]); }
  C3D operator[](unsigned int i) const { return C3D(vx[i],vy[i],vz[i]); }
  
  unsigned int size() const { return vx.size();}
  
private:
  
  AVector<T> vx,vy,vz;
  
};


#endif
