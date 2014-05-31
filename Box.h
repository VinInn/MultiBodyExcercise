#include<vector>
#include<random>
#include "Particle.h"
#include<atomic>

class Box {
public:
  Box(unsigned int nBody);
  using Float = vect3d::Float;
  using uint = unsigned int;
  using V3D =  Particle<Float>::V3D;


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
    particles[i].fill(mass,V3D{rgen(eng),rgen(eng),rgen(eng)},zero,zero);


}


void Box::computeForce() {
  //  constexpr Float eps = 0.0001;
  V3D zero = vect3d::ZERO();

  // constexpr Float fact = .1e-6;
 
  
  auto force = [=](auto a, auto b) -> V3D {
    constexpr Float eps = 0.0001;
    constexpr Float fact = .1e-7;
    
    auto && delta = b.position()-a.position();
    // very long range force
    // auto d = dist(b.position(),a.position())+eps;
    // return delta*(fact/(d*std::sqrt(d)));
    
    // long range force
    auto d2 = dist2(b.position(),a.position())+eps;
    return delta*fact/d2;

    // coulomb
    // auto d2 = dist2(b.position(),a.position())+eps;
    // return delta*(fact/(std::sqrt(d2)*d2));

    // spring
    // return delta*fact;

  };
  

  auto nBody= particles.size();

  for (auto i=0U; i< nBody; ++i) particles[i].acceleration()=zero;



  for (auto i=1U; i< nBody; ++i) {
    V3D tmp = particles[i].acceleration();
    for (auto j=0U; j< i; ++j) {
      auto && f = force(particles[i],particles[j]);
      tmp-=f;
      particles[j].acceleration()+=f;
    }
    particles[i].acceleration() = tmp;
  }

  for (auto i=0U; i< nBody; ++i) particles[i].acceleration()/=particles[i].mass();
}


void Box::evolve() {
  auto nBody= particles.size();
  for (auto i=0U; i< nBody; ++i) particles[i].update();
}


void Box::verifyBoundaries() {
  auto nBody= particles.size();
  for (auto i=0U; i< nBody; ++i) {
    for (unsigned int k=0; k<3; ++k) {
      if (particles[i].position()[k] > wallPos) particles[i].scatter(k,wallPos);
      else if (particles[i].position()[k] < -wallPos) particles[i].scatter(k,-wallPos);
    }
  }
}


Box::Float Box::temperature() const {
  Float t=0.;
  auto nBody= particles.size();
  for (auto i=0U; i< nBody; ++i) t+= mag(particles[i].velocity());
  return t/Float(particles.size());
}


#include<cassert>
void Box::check() const {
  auto nBody= particles.size();
  for (auto i=0U; i< nBody; ++i)
    for (unsigned int k=0; k<3; ++k) 
      assert(std::abs(particles[i].position()[k]) <= wallPos);

}
