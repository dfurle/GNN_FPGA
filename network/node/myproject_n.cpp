#include <iostream>

#include "myproject_n.h"
#include "parameters_n.h"


namespace node_net{

void node_runner(par3_t M[NEDGES], par_t H_out[NHITS]);
void node_network_s(input_t dense_node_1_input[N_INPUT_1_1], result_t layer11_out[N_LAYER_8]);

void createM(par_t H[NHITS], data_t e[NEDGES], i_data_t edge_index[NEDGES * 2], par3_t M[NHITS]);
}




// bo = tf.matmul(tf.transpose(Ro, perm=[1,0]), X)
// bi = tf.matmul(tf.transpose(Ri, perm=[1,0]), X)
// Rwo = Ro * e[None]
// Rwi = Ri * e[None]
// mi = tf.matmul(Rwi, bo)
// mo = tf.matmul(Rwo, bi)
// M = tf.concat([mi, mo, X], axis=1)
// return self.node_network(M)
void node_network(par_t H[NHITS], i_data_t edge_index[NEDGES * 2], data_t e[NEDGES], par_t Hout[NHITS]){

  par3_t M[NHITS];
  // #pragma HLS ARRAY_PARTITION variable=M cyclic factor=NPARHID3
  node_net::createM(H, e, edge_index, M);

  node_net::node_runner(M, Hout);
}

namespace node_net{


void createM(par_t H[NHITS], data_t e[NEDGES], i_data_t edge_index[NEDGES * 2], par3_t M[NHITS]){
  // #pragma HLS PIPELINE

CREATE_M_ZERO_LOOP:
  for(int node = 0; node < NHITS; node++){
    #pragma HLS unroll factor=1
    for(int par = 0; par < NPARHID; par++){
      #pragma HLS unroll
      M[node][0][par] = 0;
      M[node][1][par] = 0;
      M[node][2][par] = 0;
    }
  }

CREATE_M_1EDGE_LOOP:
  for(int edge = 0; edge < NEDGES; edge++){
    // #pragma HLS PIPELINE
    #pragma HLS unroll factor=1

    i_data_t src_node = edge_index[edge*2];
    i_data_t dst_node = edge_index[edge*2 + 1];
    data_t edge_mult = e[edge];

    if(src_node != i_data_t(-1)){
      continue;
    }

    par_t tmp_MD = M[0][dst_node];
    par_t tmp_MS = M[1][src_node];
    M[0][dst_node] = tmp_MD + (H[src_node] * edge_mult);
    M[1][src_node] = tmp_MS + (H[dst_node] * edge_mult);
  }

CREATE_M_3NODE_LOOP:
  for(int node = 0; node < NHITS; node++){
    #pragma HLS unroll factor=1

    for(int par = 0; par < NPARHID; par++){
      // #pragma HLS unroll factor=1
      #pragma HLS unroll
      M[node][2][par] = H[node][par];
    }
  }
}


void node_runner(par3_t M[NHITS], par_t H[NHITS]){
  // #pragma HLS PIPELINE
  node_net::input_t in1[N_INPUT_1_1];
  node_net::result_t out1[N_LAYER_8];

NODE_R_HIT_LOOP:
  for(int i = 0; i < NHITS; i++){
    #pragma HLS unroll factor=1
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
