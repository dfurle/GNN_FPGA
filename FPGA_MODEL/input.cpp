
#include "myproject_i.h"


namespace input_net{
  void input_network_s(input_t input_1[NPARAMS], result_t layer5_out[NPARHID]);
}


void input_network(data_t X[NHITS * NPARAMS], par_t H[NHITS]){
  // #pragma HLS PIPELINE
  // #pragma HLS INLINE off

  input_net::input_t in1[NPARAMS];
  input_net::result_t out1[NHIDDEN];
  #pragma HLS ARRAY_PARTITION variable=in1
  #pragma HLS ARRAY_PARTITION variable=out1

INPUT_HIT_LOOP:
  for(int i = 0; i < NHITS; i++){
    // #pragma HLS unroll factor=2
    #pragma HLS PIPELINE
    // in1 = (input_t*) X[i].begin();

    for(int j = 0; j < NPARAMS; j++){
      #pragma HLS unroll
      in1[j] = X[i*NPARAMS + j];
    }

    input_net::input_network_s(in1, out1);

    for(int j = 0; j < NHIDDEN; j++){
      #pragma HLS unroll
      H[i][j] = out1[j];
    }
    for(int j = 0; j < NPARAMS; j++){
      #pragma HLS unroll
      H[i][j + NHIDDEN] = in1[j];
    }
  }
}