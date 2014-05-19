#include "UltimateSoa.h"
#include "rdtscp.h"
#include<vector>
#include<string>


template<typename F=float, typename I=int, typename S=std::string, typename B=unsigned char, typename V = std::vector<int>> 
struct DataT {

  DataT(){}
  DataT(F ix, F iy, F iz, I is, I it, S inam, V iw, B io) :
    x(ix),y(iy), z(iz), status(is), type(it), name(inam), what(iw), ok(io){}

  template <typename D>
  DataT(D const & d) :
    x(d.x),y(d.y), z(d.z), status(d.status), type(d.type), name(d.name), what(d.what), ok(d.ok){}
  template <typename D>
  DataT & operator =(D const & d) {
    x=d.x;y=d.y; z=d.z; status=d.status; type=d.type; name=d.name; what=d.what; ok=d.ok; 
    return *this;
  }
  DataT(DataT const & d) :
    x(d.x),y(d.y), z(d.z), status(d.status), type(d.type), name(d.name), what(d.what), ok(d.ok){}
  DataT & operator =(DataT const & d) {
    x=d.x;y=d.y; z=d.z; status=d.status; type=d.type; name=d.name; what=d.what; ok=d.ok; 
    return *this;
  }

  void reset() { ok=0; } // false;}


  F x,y,z;
  I status;
  I type;
  S name;
  V what;
  B ok;

};





using Data = DataT<>;

template<>
struct UltimateSoaTraits<Data> {

  template<typename ...Args>
  static auto makeRefT(DataT<Args...>) ->  DataT<typename std::add_lvalue_reference<Args>::type...> {
      return  DataT<typename std::add_lvalue_reference<Args>::type...>();
    }

  template<typename ...Args>
  static auto makeCRefT(DataT<Args...>) ->  DataT<typename std::add_lvalue_reference<typename std::add_const<Args>>::type...> {
    return  DataT<typename std::add_lvalue_reference<typename std::add_const<Args>>::type...>();
    }


  using REF = decltype(makeRefT(Data()));
  using CREF = decltype(makeCRefT(Data()));

  using SOATUPLE = std::tuple<UltimateSoa<float,false>,UltimateSoa<float,false>,UltimateSoa<float,false>,
			      UltimateSoa<int,false>,UltimateSoa<int,false>,UltimateSoa<std::string,false>,
			      UltimateSoa<std::vector<int>,false>, UltimateSoa<unsigned char,false>
			      >; 

};

using DoD = UltimateSoa<Data,true>;

// using Cont=DoD;
template<typename Cont>
void comp(Cont & cont) {
  auto n = int(cont.size());
#pragma GCC ivdep
  for (auto i=0; i<n; ++i)
    cont[i].x = cont[i].y*cont[i].z;
}

template<typename Cont>
void reset(Cont & cont) {
  auto n = int(cont.size());
#pragma GCC ivdep
  for (auto i=0; i<n; ++i)
    cont[i].reset();
}



#include<iostream>
#include <typeinfo>
int main() {
  std::cout << typeid(Data).name() << std::endl;
  std::cout << typeid(UltimateSoaTraits<Data>::REF).name() << std::endl;
  std::cout << typeid(UltimateSoaTraits<Data>::CREF).name() << std::endl;

  DoD dod(20000);
  std::vector<Data> aos(20000);

 
  reset(dod);
  reset(aos);
  
  comp(dod);
  comp(aos);

  long long td=0, ta=0;
  for (int k=0; k<1000; ++k) {
    td -=rdtscp();
    reset(dod);
    td +=rdtscp();
    ta -=rdtscp();
    reset(aos);
    ta +=rdtscp();

  }

  std::cout << dod.size() << ' ' << double(td)/dod.size() << ' ' << double(ta)/dod.size() << std::endl;

  td=0, ta=0;
  for (int k=0; k<1000; ++k) {
    td -=rdtscp();
    comp(dod);
    td +=rdtscp();
    ta -=rdtscp();
    comp(aos);
    ta +=rdtscp();

  }

  std::cout << dod.size() << ' ' << double(td)/dod.size() << ' ' << double(ta)/dod.size() << std::endl;




  return 0;
}
