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

  using V3D = Part::V3D;

  // std::vector<Part> particles;
  Particles<Float> particles;

  if (argc < 2) {
    std::cout << "please provide number of bodies, time step, coupling-costant, file name\n" << std::endl;
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
  constexpr float v0 = 0.05f;

  Float zero = 0;
  const V3D zeroV{zero,zero,zero};
  constexpr Float mass = 1.;

  for (auto i=0U; i< nBody; ++i)
    particles[i].fill(mass,V3D{rgen(eng),rgen(eng),rgen(eng)},
		      V3D{v0*rgen(eng),v0*rgen(eng),v0*rgen(eng)},zeroV);



  long long t = 0;
  V3D forceTot=zeroV;
  V3D maxDA = zeroV;
  Float maxDV=zero;
  Float aveDV=zero;
  Float aveV=zero;
  auto iprobe = 15U;
  particles[iprobe].position() = zeroV;
  std::cout << "time/position " << t << ' ' << particles[iprobe].position()
	    << ' ' << particles[iprobe].velocity()<< std::endl;

  auto force = [=](auto a, auto b) -> V3D {
    constexpr Float eps = 0.001*0.001;
    
    auto && delta = b.position()-a.position();
    // very long range force
    // auto d = dist(b.position(),a.position())+eps;
    // return delta*(fact/(d*std::sqrt(d)));
    
    // long range force
    // auto d2 = dist2(b.position(),a.position())+eps;
    // return delta*fact/d2;
    
    // coulomb
    auto d2 = dist2(b.position(),a.position())+eps;
    return delta*(fact/(std::sqrt(d2)*d2));

    // spring
    // return delta*fact;
    

  };
  

  auto nLoop = (unsigned int)(10000/deltaT);

  // warm up
  // set initial conditions: give a deterministic "shake"
  for(auto k=0U; k<500; ++k) {
    V3D forceT=zeroV;
    for (auto i=0U; i< nBody; ++i) 
      forceT += force(particles[i],particles[iprobe]);
    particles[iprobe].acceleration()=forceT;
    particles[iprobe].update(k<50 ? 0.1 : 0.5f);
  }
  
  std::ofstream probeTraj(argc > 4 ? argv[4] :  "probeTraj.txt");
  std::ofstream temper(argc > 5 ? argv[5] :  "temperature.txt");

  for(auto k=0U; k<nLoop; ++k) {
    t -= rdtscp();
    V3D forceT=zeroV;
    //#pragma omp parallel for simd
    //#pragma omp simd
#pragma GCC ivdep
    for (auto i=0U; i< nBody; ++i) {
      auto && f = force(particles[iprobe],particles[i]);
      forceT -= f;
    }
    forceTot=forceT;
    auto dd = abs((forceTot-particles[iprobe].acceleration())/mag(particles[iprobe].velocity()));
    maxDA = max(maxDA,dd);
    maxDV = std::max(maxDV,mag(dd));
    aveDV +=mag(dd);
    aveV +=mag(particles[iprobe].velocity());

    particles[iprobe].acceleration()=forceTot;
    particles[iprobe].update(deltaT);
    t +=rdtscp();

   if (1==k%100){
     std::cout << "time/force " << t << ' ' << forceTot << std::endl;
     std::cout << "time/position " << t << ' ' << particles[iprobe].position() 
	       << ' ' << particles[iprobe].velocity()
	       << ' ' << mag(particles[iprobe].velocity())
	       << std::endl;
   }

   temper << k*deltaT << ' ' << mag(particles[iprobe].velocity()) << '\n';
#ifdef USEVECEXT
   probeTraj << particles[iprobe].position()[0]  << ' '<< particles[iprobe].position()[1]  << ' '<< particles[iprobe].position()[2] << '\n';
#else
   probeTraj << particles[iprobe].position().x()  << ' '<< particles[iprobe].position().y()  << ' '<< particles[iprobe].position().z() << '\n';
#endif
   
  }
  
  std::cout  << std::endl << "time/force/maxDA " << t << ' ' << forceTot
	     << ' '<< deltaT*maxDA << ' '<< deltaT*maxDV 
	     << ' '<< deltaT*aveDV/nLoop
	     << ' '<< aveV/nLoop  << std::endl;
  std::cout << "time/position " << double(t)/nLoop << ' ' << particles[iprobe].position() 
	    << ' ' << particles[iprobe].velocity() << ' ' << mag(particles[iprobe].velocity())<< std::endl;
 
  return 0;
};
