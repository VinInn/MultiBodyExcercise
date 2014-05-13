#include<vector>
#include<random>
#include "Particle.h"
#include<atomic>
class Box {
public:
  Box(unsigned int nBody);
  using V3D = Particle::V3D;
  using Float = Particle::Float;


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
  std::vector<Particle> particles;

#ifdef NOACC
std::vector<V3D> accelerations;
#endif

};



Box::Box(unsigned int nBody) {

  std::mt19937 eng;
  std::uniform_real_distribution<Float> rgen(-wallPos,wallPos);


  V3D zero = vect3d::ZERO();
  constexpr Float mass = 1.;

  for (auto i=0U; i< nBody; ++i)
    particles.emplace_back(mass,V3D{rgen(eng),rgen(eng),rgen(eng)},zero);

#ifdef NOACC
  accelerations.resize(nBody,zero);
#endif


}


void Box::computeForce() {
  constexpr Float eps = 0.0001;
  V3D zero = vect3d::ZERO();

  constexpr Float fact = .1e-6;
 
  auto force = [=](Particle const & a, Particle const & b) -> V3D {
    auto delta = a.position()-b.position();
    auto d = dist(a.position(),b.position())+eps;
    return delta*(fact/(d*std::sqrt(d)));
  };

  auto nBody= particles.size();
#ifdef NOACC
  for (auto & a: accelerations) a=zero;
#else
  for (auto & p: particles) p.acceleration()=zero;
#endif


  for (auto i=1U; i< nBody; ++i) {
    auto tmp = particles[i].acceleration();
    for (auto j=0U; j< i-1; ++j) {
      auto f = force(particles[j],particles[i]);
#ifdef NOACC
      accelerations[i]-=f;
      accelerations[j]+=f;
#else
      tmp-=f;
      particles[j].acceleration()+=f;
#endif
    }
    particles[i].acceleration()=tmp;
  }


#ifdef NOACC
  for (auto i=0U; i< nBody; ++i) 
    accelerations[i]/=particles[i].mass();
#else
  for (auto p: particles) p.acceleration()/=p.mass();
#endif
}






#ifdef NOACC
void Box::evolve() {
  auto i=0U;
  for (auto & p: particles) p.update(accelerations[i++]);
}
#else
void Box::evolve() {
  for (auto & p: particles) p.update();
}
#endif


void Box::verifyBoundaries() {
  for (auto & p: particles) {
    for (unsigned int i=0; i<3; ++i) {
      if (p.position()[i] > wallPos) p.scatter(i,wallPos);
      else if (p.position()[i] < -wallPos) p.scatter(i,-wallPos);
    }
  }
}


Box::Float Box::temperature() const {
  Float t=0.;
  for (auto const & p: particles) t+= mag(p.velocity());
  return t/Float(particles.size());
}


#include<cassert>
void Box::check() const {
  if(vect3d::DIM>3) 
    for (auto const & p: particles) {
      assert(std::abs(p.position()[3])==0);
      assert(std::abs(p.velocity()[3])==0);
#ifndef NOACC
      assert(std::abs(p.acceleration()[3])==0);
#endif
    }
  
  for (auto const & p: particles)
    for (unsigned int i=0; i<3; ++i) 
      assert(std::abs(p.position()[i]) <= wallPos);

}
