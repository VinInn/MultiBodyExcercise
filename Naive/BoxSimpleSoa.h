#include<vector>
#include<random>
#include "SimpleParticleSoa.h"
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
 
  
  auto force = [&](uint i, uint j) -> V3D {
    auto && delta = particles.position(j)-particles.position(i);
    auto d = dist(particles.position(j),particles.position(i))+eps;
    return delta*(fact/(d*std::sqrt(d)));
  };
  

  auto nBody= particles.size();

  for (auto i=0U; i< nBody; ++i) particles.acceleration(i)=zero;



  for (auto i=1U; i< nBody; ++i) {
    V3D tmp = particles.acceleration(i);
    for (auto j=0U; j< i-1; ++j) {
      auto && f = force(i,j);
      tmp-=f;
      particles.acceleration(j)+=f;
    }
    particles.acceleration(i) = tmp;
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
