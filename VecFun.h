#ifndef VecFun_H
#define VecFun_H

template<typename V>
inline
auto mag2(V const & a) ->decltype(dot(a,a)) {
  return dot(a,a);
} 


template<typename V>
inline
auto mag(V const & a) ->decltype(dot(a,a)) {
  return std::sqrt(dot(a,a));
} 


template<typename V1,typename V2 >
inline
auto dist2(V1 const & a, V2 const & b) ->decltype(dot(a-b,a-b)){
  auto && d = a-b;
  return dot(d,d);
  //auto pow2 = [](auto x) { return x*x;};
  //return pow2(a.x()-b.x()) + pow2(a.y()-b.y()) + pow2(a.z()-b.z());
}

template<typename V1,typename V2 >
inline
auto dist(V1 const & a, V2 const & b) ->decltype(dist2(a,b)){
  return std::sqrt(dist2(a,b));
}



#endif
