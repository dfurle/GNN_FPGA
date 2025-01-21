#include "myproject_e.h"


namespace edge_net{
  void edge_network_s(input_t input_1[NPARHID2], result_t layer7_out[1]);
}


namespace edge_net{
  void edge_runner(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t inbound[NEDGES], par_t outbound[NEDGES]);
}

void edge_network(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], int valid_edges, par_t inbound[NEDGES], par_t outbound[NEDGES]){
  edge_net::edge_runner(H, edge_index, e, inbound, outbound);

  SET_INVALID_EDGES:
  for(int i = NEDGES-1; i >= 0; i--){
    #pragma HLS UNROLL
    if(i < valid_edges)
      break;
    e[i] = data_t(0.0);
  }
}

namespace edge_net{

void edge_runner(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t inbound[NEDGES], par_t outbound[NEDGES]){
  // #pragma HLS INLINE off
  // #pragma HLS PIPELINE
  edge_net::input_t in1[NPARHID2];
  edge_net::result_t out1[1];

EDGE_R_EDGE_LOOP:
  for(int i = 0; i < NEDGES; i++){
    // #pragma HLS unroll factor=2

    // for(int j = 0; j < N_INPUT_1_1; j++){
    //   #pragma HLS unroll
    //   in1[j] = B[i][j/NPARHID][j%NPARHID];
    // }
    i_data_t src = edge_index[2*i];
    i_data_t dst = edge_index[2*i+1];

    if(src == i_data_t(-1)){
      break;
    }

    par_t Hsrc = H[src];
    par_t Hdst = H[dst];
    inbound[i] = Hsrc;
    outbound[i] = Hdst;
    for(int j = 0; j < NPARHID; j++){
      #pragma HLS UNROLL
      in1[j] = Hsrc[j];
      in1[j+NPARHID] = Hdst[j];
    }

    edge_network_s(in1, out1);

    e[i] = out1[0];

    // for(int j = 0; j < N_LAYER_6; j++){
    //   #pragma HLS unroll
    //   e[i] = out1[j];
    // }
  }
}
}