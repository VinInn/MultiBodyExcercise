float px[1024];
float vx[1024];
unsigned int N=1024;


void one(unsigned int i) {
   for (auto j=i+1; j<N; ++j) {
      auto ax = px[j]-px[i];
      vx[i]-=ax;
      vx[j]+=ax;
   }
}

void oneOK(unsigned int k) {
  auto tmp = vx[k];
   for (auto j=0; j<k-1; ++j) {
      auto ax = px[j]-px[k];
      tmp-=ax;
      vx[j]+=ax;
   }
   vx[k]=tmp;
}


void oneNope(unsigned int k) {
   for (auto j=0; j<k-1; ++j) {
      auto ax = px[j]-px[k];
      vx[k]-=ax;
      vx[j]+=ax;
   }
}
