#include<array>
#include<vector>
#include<utility>

unsigned int N;
float * a, *b, *c;

void bar() {
#pragma GCC ivdep
  for (auto i=0U; i<N; ++i)
    a[i] = b[i]*c[i];
}



template<int N>
struct SoA {

  using s_t = unsigned int;   
  using Ind = unsigned int;

  auto size() const { return m_n;}

  float & operator()(Ind i, Ind j) { return data[j][i];}
  float const & operator()(Ind i, Ind j) const { return data[j][i];}

  std::array<std::vector<float>,N> data;
  s_t m_n=0;
};


template<int N>
void doT(SoA<N> & soa) {
// #pragma GCC ivdep
  for (auto i=0U; i<soa.size(); ++i)
    soa(i,0) = soa(i,1)*soa(i,2);
}


void doIt(SoA<3> & soa) {
  doT(soa);
}
