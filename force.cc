#include <cmath>

using Float=float;

Float * __restrict__ vx;
Float * __restrict__ vy;
Float * __restrict__ vz;

Float const * __restrict__ px;
Float const * __restrict__ py;
Float const * __restrict__ pz;

unsigned int N;
Float fact, eps;

void update() {
  auto i=0U;
  //for (auto i=0U; i< N-1; ++i) {
    for (auto j=i+1; j<N; ++j) {
      auto ax = px[j]-px[i];
      auto ay = py[j]-py[i];
      auto az = pz[j]-pz[i];
      auto d = std::sqrt(ax*ax+ay*ay+az*az) + eps;
      // auto d = ax*ax; // +ay*ay+az*az;
      d = (fact/(d*std::sqrt(d)));
      ax *=d; ay *=d; az *=d;
      
      //vx[i]-=ax;
      //vy[i]-=ay;
      //vz[i]-=az;
      vx[j]+=ax;
      vy[j]+=ay;
      vz[j]+=az;
 
    }
    //}
}


void one(unsigned int i) {
   for (auto j=i+1; j<N; ++j) {
      auto ax = px[j]-px[i];
      vx[i]-=ax;
      vx[j]+=ax;
   }
}

void oneK(unsigned int k) {
  auto tmp = vx[k];
   for (auto j=0; j<k-1; ++j) {
      auto ax = px[j]-px[k];
      tmp-=ax;
      vx[j]+=ax;
   }
   vx[k]=tmp;
}


void oneN(unsigned int k) {
   for (auto j=0; j<k-1; ++j) {
      auto ax = px[j]-px[k];
      vx[k]-=ax;
      vx[j]+=ax;
   }
}


Float xx, vv;

void oneS() {
   for (auto j=0U; j<N; ++j) {
      auto ax = px[j]-xx;
      vv-=ax;
   }
}
