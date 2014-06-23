#include "SoaIterator.h"

#include <vector>
#include <array>
#include <algorithm>

#include <iostream>



int main() {


  using VI = std::vector<int>;
  VI v(10);


  using Iter = SoaIterator<int&,VI>;
  using CIter = SoaIterator<int const&,VI const>;


  int i=0;
  Iter b(v), e(v,v.size());
  for (; b!=e; ++b) *b = i++;

  CIter ce = e;
  CIter cb(v);
  i=0;
  for (; cb!=ce; ++cb) i+= *cb;

  std::cout<< i << std::endl;


  v[5]=-5;

  cb = CIter(v);
  auto m = std::min_element(cb,cb+v.size());

    std::cout<< *m << std::endl;


  return v.size()>0;

}
