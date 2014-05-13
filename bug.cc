float px[1024];
float xx, vv;
unsigned int N=1024;

void ok() {
  for (auto j=0U; j<N; ++j) {
    auto ax = px[j]-xx;
    vv-=ax;
  }
}

void noOk() {
  for (auto j=0U; j<N; ++j) {
    auto ax = xx-px[j];
    vv+=ax;
  }
}
