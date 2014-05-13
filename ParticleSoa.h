#ifndef Particle_H
#define Particle_H


#include "Vector3D.h"

template <typename T>
class Particle {
public:

  template<typename T> using V3D = Vector3D<T>;
  using value = typename std::remove_reference<T>::type;
  using ref = typename std::add_lvalue_reference<T>::type;

  using C3D = Vector3D<value const &>;
  using L3D = Vector3D<ref>;

 
  Particle(T m, V3D pos, V3D vel, V3D acc) : m_pos(pos), m_vel(vel)
				  , m_acc(acc)
				  , m_mass(m) {}

  value mass() const { return m_mass;}
  C3D position() const { return m_pos;}
  C3D velocity() const { return m_vel;}

  C3D acceleration() const { return m_acc;}
  V3D & acceleration() { return m_acc;}

  void update() {
    update(m_acc);
  }


  void scatter(unsigned int i, value wallPos) {
    m_pos[i] = wallPos - (m_pos[i]-wallPos);
    m_vel[i] = -m_vel[i];

  }

private:
  V3D m_pos;
  V3D m_vel;
  V3D m_acc;
  T m_mass;

};



class Particles {
public:

  using Float = vect3d::Float;
  using CP = Particle<Float const &>;
  using RP = Particle<Float &&>;
  using LP = Particle<Float&>;
  using Soa = SOA3D<Float>;


  explicit Particles(uint n) : m_pos(n), m_vel(n),m_acc(n),m_mass(n), m_n(n){} 

  uint size() const { return m_n;}

  CP operator[i] const { return LP(m_mass[i],m_vel[i],m_acc[i]); }
  LP operator[i]       { return LP(m_mass[i],m_vel[i],m_acc[i]); }
  


private:
  Soa m_pos;
  Soa m_vel;
  Soa m_acc;
  std::vector<Float> m_mass;
  uint m_n;

};

#endif
