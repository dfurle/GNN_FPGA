#include <iostream>

#include "myproject_n.h"
#include "parameters_n.h"


namespace node_net{

void node_runner(par3_t M[NHITS], par_t H_out[NHITS]);
void node_network_s(input_t dense_node_1_input[N_INPUT_1_1], result_t layer11_out[N_LAYER_8]);

void createM(par_t H[NHITS], data_t e[NEDGES], ei_t edge_index, par3_t M[NHITS]);
}




// bo = tf.matmul(tf.transpose(Ro, perm=[1,0]), X)
// bi = tf.matmul(tf.transpose(Ri, perm=[1,0]), X)
// Rwo = Ro * e[None]
// Rwi = Ri * e[None]
// mi = tf.matmul(Rwi, bo)
// mo = tf.matmul(Rwo, bi)
// M = tf.concat([mi, mo, X], axis=1)
// return self.node_network(M)
void node_network(par_t H[NHITS], ei_t edge_index, data_t e[NEDGES], par_t Hout[NHITS]){
  // #pragma HLS INLINE off

  par3_t M[NHITS];
  // #pragma HLS ARRAY_PARTITION variable=M
  node_net::createM(H, e, edge_index, M);

  node_net::node_runner(M, Hout);
}

namespace node_net{


void createM(par_t H[NHITS], data_t e[NEDGES], ei_t edge_index, par3_t M[NHITS]){
  // #pragma HLS INLINE off
  // #pragma HLS PIPELINE

CREATE_M_ZERO_LOOP:
  for(int node = 0; node < NHITS; node++){
    // #pragma HLS unroll factor=1
    // #pragma HLS unroll
    for(int par = 0; par < NPARHID; par++){
      #pragma HLS unroll
      M[node][0][par] = 0;
      M[node][1][par] = 0;
      M[node][2][par] = 0;
    }
  }

  // CPU/PCIe ( 2GB/s )
  //  |
  //  |
  //  | (can read ~75MB/s (or maybe only ~50MB/s))
  //   |     - (for 16 Burst Length with 64 Latency?)
  //  \-> Splitter/Returner FPGA <----------------------------\ 
  //     |                                                    |
  //     |                                                    |
  //     \-> Graph Construction                               |
  //             |                                            |
  //             |            (time estimate after pipeline)  |
  //             |                                            |
  //             \/  (~1KB of data / ~(122*5ns)) (~1.6GB/s)   |
  //         Neural Network                                   |
  //                \-----------------------------------------/


CREATE_M_1EDGE_LOOP:
  for(int edge = 0; edge < NEDGES; edge++){
    // #pragma HLS PIPELINE II=4 // was throwing errors until II=4 so TODO: try lowering? or something
    #pragma HLS PIPELINE
    
    // #pragma HLS PIPELINE off
    // #pragma HLS unroll factor=1
    // #pragma HLS unroll

    i_data_t src_node = edge_index[edge*2];
    i_data_t dst_node = edge_index[edge*2 + 1];
    data_t edge_mult = e[edge];

    if(src_node != i_data_t(-1)){
      continue;
    }

    par_t tmp_MD = M[dst_node][0];
    par_t tmp_MS = M[src_node][1];
    par_t tmp_Hs = H[src_node];
    par_t tmp_Hd = H[dst_node];

    // par_t tmp1 = tmp_MD + (tmp_Hs * edge_mult);
    // par_t tmp2 = tmp_MS + (tmp_Hd * edge_mult);
    // M[dst_node][0] = tmp1;
    // M[src_node][1] = tmp2;

    M[dst_node][0] = tmp_MD + (tmp_Hs * edge_mult);
    M[src_node][1] = tmp_MS + (tmp_Hd * edge_mult);
  }

// CREATE_M_1EDGE_LOOP_FIRST:
//   for(int edge = 0; edge < NEDGES; edge++){
//     #pragma HLS PIPELINE off
//     #pragma HLS unroll factor=1
//     // #pragma HLS unroll

//     i_data_t src_node = edge_index[edge*2];
//     i_data_t dst_node = edge_index[edge*2 + 1];
//     data_t edge_mult = e[edge];

//     if(src_node != i_data_t(-1)){
//       continue;
//     }

//     par_t tmp_MD = M[dst_node][0];
//     par_t tmp_MS = M[src_node][1];
//     M[dst_node][0] = tmp_MD + (H[src_node] * edge_mult);
//     M[src_node][1] = tmp_MS + (H[dst_node] * edge_mult);
//   }

// CREATE_M_1EDGE_LOOP_SECOND:
//   for(int edge = 0; edge < NEDGES; edge++){
//     // #pragma HLS PIPELINE
//     #pragma HLS unroll factor=1
//     // #pragma HLS unroll

//     i_data_t src_node = edge_index[edge*2];
//     i_data_t dst_node = edge_index[edge*2 + 1];
//     data_t edge_mult = e[edge];

//     if(src_node != i_data_t(-1)){
//       continue;
//     }

//     par_t tmp_MD = M[dst_node][0];
//     par_t tmp_MS = M[src_node][1];
//     M[dst_node][0] = tmp_MD + (H[src_node] * edge_mult);
//     M[src_node][1] = tmp_MS + (H[dst_node] * edge_mult);
//   }

CREATE_M_3NODE_LOOP:
  for(int node = 0; node < NHITS; node++){
    // #pragma HLS unroll factor=1
    // #pragma HLS unroll

    for(int par = 0; par < NPARHID; par++){
      // #pragma HLS unroll factor=1
      #pragma HLS unroll
      M[node][2][par] = H[node][par];
    }
  }
}


void node_runner(par3_t M[NHITS], par_t H[NHITS]){
  #pragma HLS PIPELINE
  node_net::input_t in1[N_INPUT_1_1];
  node_net::result_t out1[N_LAYER_8];

NODE_R_HIT_LOOP:
  for(int i = 0; i < NHITS; i++){
    // #pragma HLS unroll factor=1
    for(int j = 0; j < N_INPUT_1_1; j++){
      #pragma HLS unroll
      in1[j] = M[i][j/NPARHID][j%NPARHID];
    }

    node_network_s(in1, out1);

    for(int j = 0; j < N_LAYER_8; j++){
      #pragma HLS unroll
      H[i][j] = out1[j];
    }
    for(int j = 0; j < NPARAMS; j++){
      #pragma HLS unroll
      H[i][j + NHIDDEN] = in1[j + (NPARHID3 - NPARAMS)];
    }
  }
}




void node_network_s(input_t in1[N_INPUT_1_1], result_t out1[N_LAYER_8]) {
  // hls-fpga-machine-learning insert IO
  #pragma HLS ARRAY_RESHAPE variable=in1 complete dim=0
  #pragma HLS ARRAY_PARTITION variable=out1 complete dim=0
  #pragma HLS INTERFACE ap_vld port=in1, out1 
  #pragma HLS PIPELINE 


  layer2_t layer2_out[N_LAYER_2];
  #pragma HLS ARRAY_PARTITION variable=layer2_out complete dim=0
  nnet::dense<input_t, layer2_t, config2>(in1, layer2_out, w1, b1); // dense_node_1

  layer3_t layer3_out[N_LAYER_2];
  #pragma HLS ARRAY_PARTITION variable=layer3_out complete dim=0
  nnet::relu<layer2_t, layer3_t, relu_config3>(layer2_out, layer3_out); // dense_node_1_relu

  layer4_t layer4_out[N_LAYER_4];
  #pragma HLS ARRAY_PARTITION variable=layer4_out complete dim=0
  nnet::dense<layer3_t, layer4_t, config4>(layer3_out, layer4_out, w2, b2); // dense_node_2

  layer5_t layer5_out[N_LAYER_4];
  #pragma HLS ARRAY_PARTITION variable=layer5_out complete dim=0
  nnet::relu<layer4_t, layer5_t, relu_config5>(layer4_out, layer5_out); // dense_node_2_relu

  layer6_t layer6_out[N_LAYER_6];
  #pragma HLS ARRAY_PARTITION variable=layer6_out complete dim=0
  nnet::dense<layer5_t, layer6_t, config6>(layer5_out, layer6_out, w3, b3); // dense_node_3

  layer7_t layer7_out[N_LAYER_6];
  #pragma HLS ARRAY_PARTITION variable=layer7_out complete dim=0
  nnet::relu<layer6_t, layer7_t, relu_config7>(layer6_out, layer7_out); // dense_node_3_relu

  layer8_t layer8_out[N_LAYER_8];
  #pragma HLS ARRAY_PARTITION variable=layer8_out complete dim=0
  nnet::dense<layer7_t, layer8_t, config8>(layer7_out, layer8_out, w4, b4); // dense_node_out

  nnet::relu<layer8_t, result_t, relu_config11>(layer8_out, out1); // dense_node_out_relu

}

}
