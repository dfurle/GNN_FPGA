#include "input_network/firmware/myproject.h"
#include "edge_network/firmware/myproject.h"
#include "node_network/firmware/myproject.h"

void runner(data_t X_arr[NHITS * NPARAMS], data_t Ro_arr[NHITS * NEDGES], data_t Ri_arr[NHITS * NEDGES], data_t e_arr[NEDGES]){

  #ifdef STREAM
  H_t H;
  X_t X;
  R_t Ro, Ri;
  e_t e;

  data_t* ptr = (data_t*) X.begin();
  ptr = X_arr;

  hls::stream<H_t> stream_H2;
  hls::stream<H_t> stream_H;
  hls::stream<X_t> stream_X;
  hls::stream<R_t> stream_Ro;
  hls::stream<R_t> stream_Ri;
  hls::stream<e_t> stream_e;

  stream_X << X;
  input_network(stream_X, stream_H);


  // ITER #1
  stream_H >> H;

  stream_H << H;
  stream_Ro << Ro;
  stream_Ri << Ri;
  edge_network(stream_H, stream_Ro, stream_Ri, stream_e);

  stream_H << H;
  stream_Ro << Ro;
  stream_Ri << Ri;
  node_network(stream_H, stream_Ro, stream_Ri, stream_e, stream_H2);

  // ITER #2
  stream_H2 >> H;

  stream_H << H;
  stream_Ro << Ro;
  stream_Ri << Ri;
  edge_network(stream_H, stream_Ro, stream_Ri, stream_e);

  stream_H << H;
  stream_Ro << Ro;
  stream_Ri << Ri;
  node_network(stream_H, stream_Ro, stream_Ri, stream_e, stream_H2);

  // Ending

  stream_H2 >> H;

  stream_H << H;
  stream_Ro << Ro;
  stream_Ri << Ri;
  edge_network(stream_H, stream_Ro, stream_Ri, stream_e);

  stream_e >> e;
  #endif


  #ifdef VECTOR
  H_t H, H2;
  X_t X;
  R_t Ro, Ri;
  e_t e;

  data_t* ptr = (data_t*) X.begin();
  ptr = X_arr;

  input_network(X, H);
  // ITER #1
  edge_network(H, Ro, Ri, e);
  node_network(H, Ro, Ri, e, H2);
  // ITER #2
  edge_network(H, Ro, Ri, e);
  node_network(H, Ro, Ri, e, H2);
  // Ending
  edge_network(H, Ro, Ri, e);
  #endif

  
  for(int i = 0; i < NEDGES; i++){
    e_arr[i] = e[i];
  }
}