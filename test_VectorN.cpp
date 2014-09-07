#include "VectorN.h"


#include<iostream>

template<typename T> using Container = UltimateSoa<T>;


int main() {


  VectorN<float,1> a1;
  VectorN<float,3> a3;


  using V5 = VectorN<float,5>;

  VectorN<float,5> a5{1.,2.,5.,-1.,-2.};


  std::get<0>(a1) = 1.f;

  std::get<2>(a3) = std::get<0>(a1);

  std::cout << std::get<2>(a3)+std::get<3>(a5) << std::endl;

  VectorN<float&,5> rf(a5);

  std::cout << typeid(V5::tuple_type).name() << std::endl;
  std::cout << typeid(VectorN<float&,5>::tuple_type).name() << std::endl;
  std::cout << typeid(Container<V5>::Data).name() << std::endl;
  std::cout << typeid(Container<V5>::REF).name() << std::endl;
 

  std::cout << std::get<2>(rf) << ' ' << std::get<2>(a5) << std::endl;
  std::get<2>(rf) = std::get<0>(a1);
  std::cout << std::get<2>(rf) << ' ' << std::get<2>(a5) << std::endl;
  std::get<0>(a1) = 4.f;
  std::cout << std::get<2>(rf) << ' ' << std::get<2>(a5) << std::endl;
  std::get<2>(a5) = std::get<3>(a5);
  std::cout << std::get<2>(rf) << ' ' << std::get<2>(a5) << std::endl;


  std::array<float,3> a{1.f,2.f,3.f};
  VectorN<float&,3> rr(a[0],a[1],a[2]);
  std::cout << std::get<0>(rr) << ' ' << std::get<1>(rr) << ' ' << std::get<2>(rr) << std::endl;



  float & k = std::get<2>(a3);
  // VectorN<float&,3> r3(std::get<0>(a3),std::get<1>(a3),std::get<2>(a3));
  // VectorN<float&,3> r3(std::tie(std::get<0>(a3),std::get<1>(a3),std::get<2>(a3)));

  std::tuple<float&,float&,float&> r3(std::get<0>(a3),std::get<1>(a3),std::get<2>(a3));

  // std::tuple<float&,float&,float&> r3(VectorN<float,3>::t2r_impl<std::tuple<float&,float&,float&>>(a3,std::make_integer_sequence<std::size_t,3>{}));

  // VectorN<float&,3>::tuple_type rf(r3);

  //std::tuple<float&,float&,float&> r3(std::tie(std::get<0>(a3),std::get<1>(a3),std::get<2>(a3)));

  std::get<2>(r3) = 3.4;

  Container<V5> cont; cont.resize(32);
  Container<float> f1; f1.resize(32);
  Container<float> f2; f2.resize(32);

  for (auto i=0U; i<cont.size(); ++i) { f1[i]=i; f2[i] = -10*int(i);}
  std::cout << cont.size() << ", " << f1[12] << ' ' << f2[12] << std::endl;
  
  for (auto i=0U; i<cont.size(); ++i) {
    std::get<0>(cont[i]) = f1[i]+f2[i];
    std::get<2>(cont[i]) = f1[i]+f2[i];
    std::get<3>(cont[i]) = f1[i]-f2[i];
  }
  

  cont[4] = a5;
  std::cout  << std::endl;
  std::cout << std::get<3>(cont[4]) << ' ' << std::get<3>(a5) << std::endl;
  std::cout << std::get<0>(cont[4]) << ' ' << std::get<0>(a5) << std::endl;


  std::cout << std::get<2>(a5) << std::endl;
  std::cout << std::get<2>(r3) << std::endl;
  std::get<2>(a3) = -7.3;
  std::cout << std::get<2>(r3) << std::endl;
  std::cout << std::get<3>(cont[12]) << std::endl;
  std::cout << std::get<0>(cont[12]) << std::endl;

  return 0;
};
