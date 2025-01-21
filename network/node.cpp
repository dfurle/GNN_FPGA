#include "myproject_n.h"


namespace node_net{
  void node_network_s(input_t input_1[NPARHID3], result_t layer5_out[NPARHID]);
}

namespace node_net{
  void node_runner(i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t inbound[NEDGES], par_t outbound[NEDGES], par_t H[NHITS], par_t H_out[NHITS]);
}

void node_network(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t H_out[NHITS], par_t inbound[NEDGES], par_t outbound[NEDGES]){
  node_net::node_runner(edge_index, e ,inbound, outbound, H, H_out);
}

namespace node_net{

void node_runner(i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t inbound[NEDGES], par_t outbound[NEDGES], par_t H[NHITS], par_t H_out[NHITS]){
  // #pragma HLS PIPELINE
  node_net::input_t in1[NPARHID3];
  node_net::result_t out1[NPARHID];
  #pragma HLS ARRAY_PARTITION variable=in1 complete dim=0
  #pragma HLS ARRAY_PARTITION variable=out1 complete dim=0

  NODE_R_HIT_LOOP:
  for(int i = 0; i < NHITS; i++){
    #pragma HLS PIPELINE
    // #pragma HLS unroll

    // for(int j = 0; j < NPARHID; j++){
    //   #pragma HLS unroll
    //   in1[j]           = M[0 * NHITS + i][j];
    //   in1[j+NPARHID]   = M[1 * NHITS + i][j];
    //   in1[j+NPARHID*2] = H[i][j];
    // }

    par_t sum_in(0);
    par_t sum_out(0);

    // printf("NODE [%d] ---------\n", i);

    for(int edge = 0; edge < NEDGES; edge++){
      // #pragma HLS PIPELINE
      i_data_t src = edge_index[2*edge];
      i_data_t dst = edge_index[2*edge+1];

      if(src == i_data_t(-1)){
        break;
      }

      data_t e_mult = e[edge];
      par_t in = inbound[edge];
      par_t out = outbound[edge];

      if(src == i_data_t(i)){
        // printf("edge (src, sum_out) %d --> [%d, %d]\n", edge, src, dst);
        // printf(" ");
        // for(int i = 0; i < NPARHID; i++){
        //   printf("%.3f, ", float(out[i]));
        // }
        // printf("\n");
        sum_out = sum_out + out * e_mult;
      } else if(dst == i_data_t(i)){
        // printf("edge (dst, sum_in ) %d --> [%d, %d]\n", edge, src, dst);
        // printf(" ");
        // for(int i = 0; i < NPARHID; i++){
        //   printf("%.3f, ", float(in[i]));
        // }
        // printf("\n");
        sum_in = sum_in + in * e_mult;
      }

    }
    // printf("\n");

    // printf("sum_in[%d]:\n", i);
    // for(int i = 0; i < NPARHID; i++){
    //   printf("%.3f, ", float(sum_in[i]));
    // }
    // printf("\n");
    // printf("sum_out[%d]:\n", i);
    // for(int i = 0; i < NPARHID; i++){
    //   printf("%.3f, ", float(sum_out[i]));
    // }
    // printf("\n");

    AGGREGATE_TO_IN1:
    for(int j = 0; j < NPARHID; j++){
      #pragma HLS unroll
      in1[j]           = sum_in[j];
      in1[j+NPARHID]   = sum_out[j];
      in1[j+NPARHID*2] = H[i][j];
    }

    // printf("Input:\n");
    // for(int i = 0; i < NPARHID3; i++){
    //   printf("%.3f, ", float(in1[i]));
    // }
    // printf("\n\n");

    node_network_s(in1, out1);

    for(int j = 0; j < NHIDDEN; j++){
      #pragma HLS unroll
      H_out[i][j] = out1[j];
    }
    for(int j = 0; j < NPARAMS; j++){
      #pragma HLS unroll
      H_out[i][j + NHIDDEN] = in1[j + (NPARHID3 - NPARAMS)];
    }
  }
}
}