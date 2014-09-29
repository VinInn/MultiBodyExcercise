#include<vector>
#include<random>
#include "Box.h"


#include<iostream>
#include<cstring>

#include <omp.h>
#include "rdtscp.h"


#include<fstream>

int 
main(int argc, char* argv[]){
  
  using Float = Box::Float;

  if (argc < 2) {
    std::cout << "please provide number of bodies, time step, coupling-costant, file name\n" << std::endl;
    exit(-1);
  }
  

  unsigned int nBody = ::atoi(argv[1]);

  Float deltaT = 1.f;
  float fact = 1.e-8;

  if (argc > 2) deltaT = ::atof(argv[2]);
  if (argc > 3) fact = ::atof(argv[3]);



  Box box(nBody,fact);
  
  box.check();


  std::cout << "start " << nBody << ' ' << deltaT << ' ' << fact<< std::endl;





  long long t = 0;
  auto iprobe = nBody/2;

  {
  auto const & probe = box.particles()[iprobe];

  std::cout << "time/position " << t << ' ' << probe.position()
	    << ' ' << probe.velocity()<< std::endl;

  }
  constexpr float tMax = 10000.;

 
  std::ofstream probeTraj(argc > 4 ? argv[4] :  "probeTraj.txt");
  std::ofstream probeSpeed(argc > 5 ? argv[5] :  "speed.txt");
  std::ofstream temper(argc > 5 ? argv[5] :  "temperature.txt");
  std::ofstream cloud(argc > 5 ? argv[5] :  "cloud.txt");

  float tTot=0;
  auto k=0U;
  while (tTot<tMax) {
    // auto dT = std::min(.1f,deltaT/(0.0001f+mag(particles[iprobe].velocity())));
    t -= rdtscp();
    auto dT = deltaT;
    tTot+=dT;
    box.oneStep(dT);
    t +=rdtscp();
    
    auto const & probe = box.particles()[iprobe];

    if (1==k%100){
      std::cout << "time/time/temp " << t << ' ' << tTot<< ' ' << dT <<' ' << box.temperature() << std::endl;
      std::cout << "time/position " << t << ' ' << probe.position() 
		<< ' ' << probe.velocity()
		<< ' ' << mag(probe.velocity())
		<< std::endl;
    }
    
    probeSpeed << k*deltaT << ' ' << mag(probe.velocity()) << '\n';
    temper << k*deltaT << ' ' << box.temperature() << '\n';
#ifdef USEVECEXT
    probeTraj << probe.position()[0]  << ' '<< probe].position()[1]  << ' '<< probe.position()[2] << '\n';
#else
    probeTraj << probe.position().x()  << ' '<< probe.position().y()  << ' '<< probe.position().z() << '\n';
#endif
    ++k;
  }
  
  for (auto i=0U; i<box.particles().size(); ++i) {
      auto const & p = box.particles()[i];
      if (std::abs(p.position().x())<1.f && std::abs(p.position().y())<1.f && std::abs(p.position().z())<1.f)
      cloud << p.position().x()  << ' '<< p.position().y()  << ' '<< p.position().z() << '\n';
    }

  auto const & probe = box.particles()[iprobe];
  std::cout  << std::endl << "time/nloop/temp " << t << ' ' << k << ' ' << box.temperature()
	     << ' ' << std::endl;
  std::cout << "time/position " << double(t)/k << ' ' << probe.position() 
	    << ' ' << probe.velocity() << ' ' << mag(probe.velocity())<< std::endl;
 
  return 0;
};
