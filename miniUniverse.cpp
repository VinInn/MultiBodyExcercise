#include<vector>
#include<random>
#include "Particle.h"


#include<iostream>
#include<cstring>

#include <omp.h>
#include "rdtscp.h"


#include<fstream>

int 
main(int argc, char* argv[]){
  
#ifdef  USEDOUBLE
  using Float = double;
#else
  using Float = float;
#endif


  using Part = Particle<Float>;
  // using PartV = Particle<Float, Part::V4V>;

  using V3D = Part::V3D;

  // std::vector<Part> particles;
  Particles<Float> particles;  // "vector of particles"

  //if (argc < 2) {
  //  std::cout << "please provide number of particles, time step, coupling-costant, file name\n" << std::endl;
  //  exit(-1);
  // }
  

  unsigned int nBody = 1024;

  Float deltaT = 0.05f;
  Float kforce = 10.f;

  if (argc > 1) nBody = ::atoi(argv[1]);
  if (argc > 2) deltaT = ::atof(argv[2]);
  if (argc > 3) kforce = ::atof(argv[3]);

  std::cout << "start " << nBody << ' ' << deltaT << ' ' << kforce << std::endl;

  particles.resize(nBody);

  constexpr Float wallPos = 1.f;

  std::mt19937 eng;
  std::uniform_real_distribution<float> rgen(-wallPos,wallPos);

  V3D zero = vect3d::ZERO();

  constexpr Float mass = 1.;


  constexpr Float speed = 0.0001;


  for (auto i=0U; i< nBody; ++i)
    particles[i].fill(mass,V3D{rgen(eng),rgen(eng),rgen(eng)},
		      speed*V3D{rgen(eng),rgen(eng),rgen(eng)},zero);


  long long t = 0;


  
  
  auto force = [&](auto a, auto b, Float & ep) -> V3D {
    constexpr Float eps = 0.00001;
    const Float fact = -1.e-9*kforce;
    
    auto && delta = b.position()-a.position();

    // coulomb
    auto d2 = dist2(b.position(),a.position())+eps;
    ep += a.mass()*b.mass()*fact/std::sqrt(d2);
    return a.mass()*b.mass()*delta*(fact/(std::sqrt(d2)*d2));

  };



  auto nLoop = (unsigned int)(1000/deltaT);


  for(auto k=0U; k<nLoop; ++k) {

    t -= rdtscp();
    for (auto i=0U; i< nBody; ++i) particles[i].acceleration()=zero;

    Float pEn=0;
    for (auto i=1U; i< nBody; ++i) {
      V3D tmp = particles[i].acceleration();
      for (auto j=0U; j< i; ++j) {
	auto && f = force(particles[i],particles[j],pEn);
	tmp-=f;
	particles[j].acceleration()+=f;
      }
      particles[i].acceleration() = tmp;
    }
    for (auto i=0U; i< nBody; ++i) particles[i].acceleration()/=particles[i].mass();


    Float kEn=0.;
    for (auto i=0U; i< nBody; ++i) kEn+= particles[i].mass()*mag2(particles[i].velocity());
    kEn *=0.5f;
    
    for (auto i=0U; i< nBody; ++i) particles[i].update(deltaT);

    t +=rdtscp();

    if (1==k%100){
      std::cout << "time/energies " << t << ' ' << pEn << ' ' << kEn << std::endl;
    }
    
  
  }


  std::cout << "time " << 1.e-6*double(t)/nLoop  << std::endl;

  return 0;

}


