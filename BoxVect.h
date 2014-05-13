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


  constexpr unsigned int NG = 16;
  V3D acc[NG];

 
  auto force = [&](Particle const & a, Particle const * b, unsigned int N) -> void {
    Float d[N];
    for (auto i=0U; i<N; ++i) {
      acc[i] = a.position()-b[i].position();
      d[i] = dist2(a.position(),b[i].position())+eps;
    }
    for (auto i=0U; i<N; ++i) d[i] = (fact/(d[i]*std::sqrt(d[i])));
    for (auto i=0U; i<N; ++i) acc[i]*= d[i];
  };
  
  unsigned int nBody= particles.size();
#ifdef NOACC
  for (auto & a: accelerations) a=zero;
#else
  for (auto & p: particles) p.acceleration()=zero;
#endif

  
  for (auto i=0U; i< nBody-1; ++i) {
    for (auto j=i+1; j< nBody; j+=NG) {
      auto N = std::min(NG,nBody-j);
      force(particles[i],&particles[j],N);
      for (auto l=0U; l<N; ++l) {
#ifdef NOACC
	 accelerations[i]+=acc[l];
	 accelerations[j+l]-=acc[l];
#else
      particles[i].acceleration()+=acc[l];
      particles[j+l].acceleration()-=acc[l];
#endif
      }
    }
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
