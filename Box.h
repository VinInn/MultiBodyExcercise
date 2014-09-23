#include<vector>
#include<random>
#include "Particle.h"
#include<atomic>


class Box {
public:
  Box(unsigned int nBody);
#ifdef  USEDOUBLE
  using Float = double;
#else
  using Float = float;
#endif

  using uint = unsigned int;
  using V3D =  Particle<Float>::V3D;


  void oneStep(Float dt) {
    computeForce();
    evolve(dt);
    verifyBoundaries();
  }

  void check() const;

  Float temperature() const;

  Particles<float> const particles() const { return m_particles;}


private:

  void computeForce();
  void evolve(Float dt);
  void verifyBoundaries();


  Float wallPos = 1.;
  Particles<float> m_particles;


};



Box::Box(unsigned int nBody) : m_particles(nBody){

  std::mt19937 eng;
  std::uniform_real_distribution<Float> rgen(-wallPos,wallPos);


  V3D zero = vect3d::ZERO();
  constexpr Float mass = 1.;

  constexpr Float speed = 0.001;

  unsigned int nHeavy = nBody/50;
  for (auto i=0U; i< nHeavy; ++i)
    m_particles[i].fill(20.f*mass,V3D{rgen(eng),rgen(eng),rgen(eng)},
			speed/1.f*V3D{rgen(eng),rgen(eng),rgen(eng)},zero);
  for (auto i=nHeavy; i< nBody; ++i)
    m_particles[i].fill(mass,V3D{rgen(eng),rgen(eng),rgen(eng)},
			speed/1.f*V3D{rgen(eng),rgen(eng),rgen(eng)},zero);


}


void Box::computeForce() {
  V3D zero = vect3d::ZERO();
 
  
  auto force = [=](auto a, auto b) -> V3D {
    constexpr Float eps = 0.00001;
    constexpr Float fact = -1.e-8;
    
    auto && delta = b.position()-a.position();
    // very long range force
    // auto d = dist(b.position(),a.position())+eps;
    // return delta*(fact/(d*std::sqrt(d)));
    
    // long range force
    //auto d2 = dist2(b.position(),a.position())+eps;
    //return a.mass()*b.mass()*delta*fact/d2;

    // coulomb
    auto d2 = dist2(b.position(),a.position())+eps;
    return a.mass()*b.mass()*delta*(fact/(std::sqrt(d2)*d2));

    // spring
    // return delta*fact;

  };
  

  auto nBody= m_particles.size();

  for (auto i=0U; i< nBody; ++i) m_particles[i].acceleration()=zero;



  for (auto i=1U; i< nBody; ++i) {
    V3D tmp = m_particles[i].acceleration();
    for (auto j=0U; j< i; ++j) {
      auto && f = force(m_particles[i],m_particles[j]);
      tmp-=f;
      m_particles[j].acceleration()+=f;
    }
    m_particles[i].acceleration() = tmp;
  }

  for (auto i=0U; i< nBody; ++i) m_particles[i].acceleration()/=m_particles[i].mass();
}


void Box::evolve(Float dt) {
  auto nBody= m_particles.size();
  for (auto i=0U; i< nBody; ++i) m_particles[i].update(dt);
}


void Box::verifyBoundaries() {
  auto nBody= m_particles.size();
  for (auto i=0U; i< nBody; ++i) {
    for (unsigned int k=0; k<3; ++k) {
      if (m_particles[i].position()[k] > wallPos) m_particles[i].scatter(k,wallPos);
      else if (m_particles[i].position()[k] < -wallPos) m_particles[i].scatter(k,-wallPos);
    }
  }
}


Box::Float Box::temperature() const {
  Float t=0.;
  auto nBody= m_particles.size();
  for (auto i=0U; i< nBody; ++i) t+= mag(m_particles[i].velocity());
  return t/Float(m_particles.size());
}


#include<cassert>
void Box::check() const {
  auto nBody= m_particles.size();
  for (auto i=0U; i< nBody; ++i)
    for (unsigned int k=0; k<3; ++k) 
      assert(std::abs(m_particles[i].position()[k]) <= wallPos);

}
