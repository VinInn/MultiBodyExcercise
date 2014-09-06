#ifndef Vector_H
#define Vector_H
#include <tuple>
#include<cmath>
#include<algorithm>
#include<type_traits>




template<typename T, typename TUPLE>
auto cat(T t, TUPLE tuple) { return std::tuple_cat(std::tie(t),tuple);}

template<typename T, int N>
class VectorN : public decltype(std::tuple_cat(std::tuple<T>(), typename VectorN<T,N-1>::tuple_type())) {
public:
  using tuple_type = decltype(std::tuple_cat(std::tuple<T>(), typename VectorN<T,N-1>::tuple_type()));

  static constexpr unsigned int DIM=N;
  using value = typename std::remove_const<typename std::remove_reference<T>::type>::type;
  using  ref = typename std::add_lvalue_reference<T>::type;
  using cref = typename std::add_lvalue_reference<typename std::add_const< T>::type>::type;
  using VAL = VectorN<value,DIM>;
  using CREF = VectorN<cref,DIM>;
  using REF = VectorN<ref,DIM>;

};



template<typename T>
class VectorN<T,1> : public std::tuple<T> {
public:
  using tuple_type = std::tuple<T>;
  static constexpr unsigned int DIM=1;
  using value = typename std::remove_const<typename std::remove_reference<T>::type>::type;
  using  ref = typename std::add_lvalue_reference<T>::type;
  using cref = typename std::add_lvalue_reference<typename std::add_const< T>::type>::type;
  using VAL = VectorN<value,DIM>;
  using CREF = VectorN<cref,DIM>;
  using REF = VectorN<ref,DIM>;

};


#endif
