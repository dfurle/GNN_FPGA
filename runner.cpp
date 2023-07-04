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

  for(int i = 0; i < NEDGES; i++){
    e_arr[i] = e[i];
  }
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
  edge_network(H2, Ro, Ri, e);
  node_network(H2, Ro, Ri, e, H);
  // Ending
  edge_network(H, Ro, Ri, e);
  for(int i = 0; i < NEDGES; i++){
    e_arr[i] = e[i];
  }
  #endif

  #ifdef ARRAY
  data_t H_arr[NHITS * NPARHID];
  data_t H2_arr[NHITS * NPARHID];

  input_network(X_arr, H_arr);
  // ITER #1
  edge_network(H_arr, Ro_arr, Ri_arr, e_arr);
  node_network(H_arr, Ro_arr, Ri_arr, e_arr, H2_arr);
  // ITER #2
  edge_network(H2_arr, Ro_arr, Ri_arr, e_arr);
  node_network(H2_arr, Ro_arr, Ri_arr, e_arr, H_arr);
  // Ending
  edge_network(H_arr, Ro_arr, Ri_arr, e_arr);
  #endif
  
}