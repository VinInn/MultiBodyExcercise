#ifndef Particle_H
#define Particle_H


#include "Vector3D.h"


template <typename T>
class Particle {
public:

  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;

#ifdef USEVECEXT
  using T3D = Vec4D<value>;
  using V3D = Vec4D<value>;
  using C3D = Vec4D<value>;
#else
  using T3D = Vector3D<T>;
  using V3D = Vector3D<value>;
  using C3D = Vector3D<value const &>;
  using L3D = Vector3D<ref>;
#endif


  Particle(){}
  Particle(T m, T3D pos, T3D vel, T3D acc) : m_pos(pos), m_vel(vel)
				  , m_acc(acc)
				  , m_mass(m) {}


  void fill (value m, V3D pos, V3D vel, V3D acc) {
    m_pos=pos; m_vel=vel; m_acc=acc; m_mass=m;
  }


  value mass() const { return m_mass;}
  T3D const & position() const { return m_pos;}
  T3D const & velocity() const { return m_vel;}

  T3D const & acceleration() const { return m_acc;}
  T3D & acceleration() { return m_acc;}

  void update() {
    C3D a =  m_acc*value(0.5);
    m_pos += m_vel + a;
    m_vel += m_acc;

  }


  void scatter(unsigned int i, value wallPos) {
    m_pos[i] = wallPos - (m_pos[i]-wallPos);
    m_vel[i] = -m_vel[i];

  }

private:
  T3D m_pos;
  T3D m_vel;
  T3D m_acc;
  T m_mass;

};

// #define  USESOA

#ifdef USESOA
class Particles {
public:

  using Float = vect3d::Float;
  using CP = Particle<Float const &>;
  using RP = Particle<Float &&>;
  using LP = Particle<Float&>;
  using Soa = SOA3D<Float>;
  using uint = unsigned int;

  explicit Particles(uint n) : m_pos(n), m_vel(n),m_acc(n),m_mass(n), m_n(n){} 

  uint size() const { return m_n;}

  CP operator[](uint i) const { return CP(m_mass[i],m_pos[i],m_vel[i],m_acc[i]); }
  LP operator[](uint i)       { return LP(m_mass[i],m_pos[i],m_vel[i],m_acc[i]); }
  


private:
  Soa m_pos;
  Soa m_vel;
  Soa m_acc;
  AVector<Float> m_mass;
  uint m_n;

};

#else
using Particles = std::vector<Particle<vect3d::Float>>; 
#endif



#endif
