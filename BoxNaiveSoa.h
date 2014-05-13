#include<vector>
#include<random>
#include "ParticleSoa.h"
#include<atomic>

class Box {
public:
  Box(unsigned int nBody);
  using V3D = Particles::V3D;
  using R3D = Particles::R3D;
  using Soa = Particles::Soa;
  using Float = Particles::Float;
  using uint = Particles::uint;

  void oneStep() {
    computeForce();
    evolve();
    verifyBoundaries();
  }

  void check() const;

  Float temperature() const;


private:

  void computeForce();
  void evolve();
  void verifyBoundaries();


  Float wallPos = 1.;
  Particles particles;


};



Box::Box(unsigned int nBody) : particles(nBody){

  std::mt19937 eng;
  std::uniform_real_distribution<Float> rgen(-wallPos,wallPos);


  V3D zero = vect3d::ZERO();
  constexpr Float mass = 1.;

  for (auto i=0U; i< nBody; ++i)
    particles.fill(i,mass,V3D(rgen(eng),rgen(eng),rgen(eng)),zero);


}


void Box::computeForce() {
  constexpr Float eps = 0.0001;
  V3D zero = vect3d::ZERO();

  constexpr Float fact = .1e-6;
 

  auto nBody= particles.size();

  for (auto i=0U; i< nBody; ++i) particles.acceleration(i)=zero;


  Float * __restrict__ vx = &particles.acceleration(0).x();
  Float * __restrict__ vy = &particles.acceleration(0).y();
  Float * __restrict__ vz = &particles.acceleration(0).z();

  Float const * __restrict__ px = &particles.position(0).x();
  Float const * __restrict__ py = &particles.position(0).y();
  Float const * __restrict__ pz = &particles.position(0).z();


  for (auto i=1U; i< nBody; ++i) {
    auto tx = vx[i]; auto ty = vy[i]; auto tz = vz[i];
    for (auto j=0U; j< i-1; ++j) {
      auto ax = px[j]-px[i];
      auto ay = py[j]-py[i];
      auto az = pz[j]-pz[i];
      auto d = std::sqrt(ax*ax+ay*ay+az*az) + eps;
      d = (fact/(d*std::sqrt(d)));
      ax *=d; ay *=d; az *=d;


      tx-=ax;
      ty-=ay;
      tz-=az;
      vx[j]+=ax;
      vy[j]+=ay;
      vz[j]+=az;
 
    }
    vx[i]=tx; ; vy[i]=ty; vz[i]=tz;
 
  }

  for (auto i=0U; i< nBody; ++i) particles.acceleration(i)/=particles.mass(i);
}


void Box::evolve() {
  auto nBody= particles.size();
  for (auto i=0U; i< nBody; ++i) particles.update(i);
}


void Box::verifyBoundaries() {
  auto nBody= particles.size();
  for (auto i=0U; i< nBody; ++i) {
    for (unsigned int k=0; k<3; ++k) {
      if (particles.position(i)[k] > wallPos) particles.scatter(i,k,wallPos);
      else if (particles.position(i)[k] < -wallPos) particles.scatter(i,k,-wallPos);
    }
  }
}


Box::Float Box::temperature() const {
  Float t=0.;
  auto nBody= particles.size();
  for (auto i=0U; i< nBody; ++i) t+= mag(particles.velocity(i));
  return t/Float(particles.size());
}


#include<cassert>
void Box::check() const {
  auto nBody= particles.size();
  for (auto i=0U; i< nBody; ++i)
    for (unsigned int k=0; k<3; ++k) 
      assert(std::abs(particles.position(i)[k]) <= wallPos);

}
