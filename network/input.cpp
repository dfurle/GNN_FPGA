
#include "myproject_i.h"


namespace input_net{
  void input_network_s(input_t input_1[N_INPUT_1_1], result_t layer5_out[N_LAYER_4]);
}


void input_network(data_t X[NHITS * NPARAMS], par_t H[NHITS]){
  // #pragma HLS PIPELINE
  // #pragma HLS INLINE off

  input_net::input_t in1[N_INPUT_1_1];
  input_net::result_t out1[N_LAYER_4];
  #pragma HLS ARRAY_PARTITION variable=in1
  #pragma HLS ARRAY_PARTITION variable=out1

INPUT_HIT_LOOP:
  for(int i = 0; i < NHITS; i++){
    // #pragma HLS unroll factor=2
    #pragma HLS PIPELINE
    // in1 = (input_t*) X[i].begin();

    for(int j = 0; j < N_INPUT_1_1; j++){
      #pragma HLS unroll
      in1[j] = X[i*N_INPUT_1_1 + j];
    }

    input_net::input_network_s(in1, out1);

    for(int j = 0; j < N_LAYER_4; j++){
      #pragma HLS unroll
      H[i][j] = out1[j];
    }
    for(int j = 0; j < N_INPUT_1_1; j++){
      #pragma HLS unroll
      H[i][j + N_LAYER_4] = in1[j];
    }
  }
}