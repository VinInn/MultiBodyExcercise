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
  
  using Float = vect3d::Float;

  using Part = Particle<Float>;

  using V3D = Part::V3D;

  // std::vector<Part> particles;
  Particles particles;  // "vector of particles"

  if (argc < 2) {
    std::cout << "please provide number of particles, time step, coupling-costant, file name\n" << std::endl;
    exit(-1);
  }
  

  unsigned int nBody = ::atoi(argv[1]);

  Float deltaT = 1.f;
  Float fact = 1.e-8;

  if (argc > 2) deltaT = ::atof(argv[2]);
  if (argc > 3) fact = 1.e-9*::atof(argv[3]);

  std::cout << "start " << nBody << ' ' << deltaT << ' ' << fact<< std::endl;

  particles.resize(nBody);

  constexpr Float wallPos = 1.f;

  std::mt19937 eng;
  std::uniform_real_distribution<float> rgen(-wallPos,wallPos);

  constexpr Float zero = 0;
  constexpr V3D zeroV{zero,zero,zero};
  constexpr Float mass = 1.;

  constexpr Float speed = 0.05;

  for (auto i=0U; i< nBody; ++i)
    particles[i].fill(mass,V3D{rgen(eng),rgen(eng),rgen(eng)},
		      speed*V3D{rgen(eng),rgen(eng),rgen(eng)},zeroV);


  long long t = 0;


  // "probe particle"  (just one)
  auto iprobe = 15U;
  particles[iprobe].position() = zeroV;
  std::cout << "time/position/velocity " << t << ' ' << particles[iprobe].position()
	    << ' ' << particles[iprobe].velocity()<< std::endl;


  auto force = [=](auto a, auto b) -> V3D {
    return fact*V3D{0,0,0};
  };


  auto nLoop = (unsigned int)(1000/deltaT);

  // warm up
  // set initial conditions: give a deterministic "shake"
  for(auto k=0U; k<500; ++k) {
    auto dt = k<50 ? 0.1 : 0.5f;
    V3D forceT=zeroV;
    for (auto i=0U; i< nBody; ++i) 
      forceT += force(particles[i],particles[iprobe]);
    particles[iprobe].acceleration()=forceT;
    particles[iprobe].update(dt);
  }

  Float aveV=zero;

  std::ofstream probeTraj(argc > 4 ? argv[4] :  "probeTraj.txt");

  for(auto k=0U; k<nLoop; ++k) {
    t -= rdtscp();
    V3D forceT=zeroV;
    for (auto i=0U; i< nBody; ++i) 
      forceT -= force(particles[iprobe],particles[i]);
 
    particles[iprobe].acceleration()=forceT;
    particles[iprobe].update(deltaT);

    aveV +=mag(particles[iprobe].velocity());
    t +=rdtscp();

   if (1==k%100){
     std::cout << "time/force " << t << ' ' << forceT << std::endl;
     std::cout << "time/position " << t << ' ' << particles[iprobe].position() 
	       << ' ' << particles[iprobe].velocity()
	       << ' ' << mag(particles[iprobe].velocity())
	       << std::endl;
   }
    
#ifdef USEVECEXT
   probeTraj << particles[iprobe].position()[0]  << ' '<< particles[iprobe].position()[1]  << ' '<< particles[iprobe].position()[2] << '\n';
#else
   probeTraj << particles[iprobe].position().x()  << ' '<< particles[iprobe].position().y()  << ' '<< particles[iprobe].position().z() << '\n';
#endif
   
   }


  std::cout << "time/position/velocity/ave-vel " << 1.e-6*double(t)/nLoop << ' ' << particles[iprobe].position() 
	    << ' ' << particles[iprobe].velocity() << ' ' << mag(particles[iprobe].velocity())
	    << ' '<< aveV/nLoop  << std::endl;

  return 0;

}


