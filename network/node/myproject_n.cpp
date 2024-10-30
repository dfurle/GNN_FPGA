#include <iostream>

#include "myproject_n.h"
#include "parameters_n.h"


namespace node_net{

void node_runner(i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t inbound[NEDGES], par_t outbound[NEDGES], par_t H[NHITS], par_t H_out[NHITS]);
// void node_runner(par_t M[2 * NHITS], par_t H[NHITS], par_t H_out[NHITS]);
// void node_runner(par_t M[3 * NHITS], par_t H[NHITS], par_t H_out[NHITS]);
// void node_runner(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t H_out[NHITS]);
void node_network_s(input_t dense_node_1_input[N_INPUT_1_1], result_t layer11_out[N_LAYER_8]);

void createM(par_t H[NHITS], data_t e[NEDGES], i_data_t edge_index[NEDGES*2], par_t inbound[NEDGES], par_t outbound[NEDGES], par_t M[3 * NHITS]);
}




// bo = tf.matmul(tf.transpose(Ro, perm=[1,0]), X)
// bi = tf.matmul(tf.transpose(Ri, perm=[1,0]), X)
// Rwo = Ro * e[None]
// Rwi = Ri * e[None]
// mi = tf.matmul(Rwi, bo)
// mo = tf.matmul(Rwo, bi)
// M = tf.concat([mi, mo, X], axis=1)
// return self.node_network(M)
void node_network(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t H_out[NHITS], par_t inbound[NEDGES], par_t outbound[NEDGES]){
  // #pragma HLS INLINE off

  // par_t M[2 * NHITS];
  // #pragma HLS ARRAY_PARTITION variable=M block factor=2
  // node_net::createM(H, e, edge_index, inbound, outbound, M);

  // node_net::node_runner(H, edge_index, e, Hout);
  // node_net::node_runner(inbound, outbound, H, H_out);
  // node_net::node_runner(M, H, H_out);
  node_net::node_runner(edge_index, e ,inbound, outbound, H, H_out);
}

namespace node_net{


// void dot_bound_T_select(par_t bound[NEDGES], data_t selector[NEDGES], par_t& aggregate){
//   #pragma HLS PIPELINE
//   #pragma HLS ARRAY_PARTITION variable=bound complete dim=0
//   #pragma HLS ARRAY_PARTITION variable=selector complete dim=0
//   for(int i = 0; i < NPARHID; i++){
//     // #pragma HLS UNROLL
//     data_t sum = 0;
//     for(int j = 0; j < NEDGES; j++){
//       // #pragma HLS UNROLL
//       sum = sum + bound[j][i] * selector[j];
//     }
//     aggregate[i] = sum;
//   }
// }

// void createM(par_t H[NHITS], data_t e[NEDGES], i_data_t edge_index[NEDGES*2], par_t inbound[NEDGES], par_t outbound[NEDGES], par_t M[3 * NHITS]){
//   // #pragma HLS INLINE off
//   // #pragma HLS PIPELINE
//   #pragma HLS ARRAY_PARTITION variable=edge_index complete dim=0
//   #pragma HLS ARRAY_PARTITION variable=e complete dim=0
//   #pragma HLS ARRAY_PARTITION variable=inbound complete dim=0
//   #pragma HLS ARRAY_PARTITION variable=outbound complete dim=0

//   CREATE_M_ZERO_LOOP:
//   for(int node = 0; node < NHITS; node++){
//     #pragma HLS PIPELINE
//     // #pragma HLS unroll factor=1
//     // #pragma HLS unroll
//     for(int par = 0; par < NPARHID; par++){
//       #pragma HLS UNROLL
//       M[0*NHITS + node][par] = 0;
//       M[1*NHITS + node][par] = 0;
//     }
//   }

//   // CPU/PCIe ( 2GB/s )
//   //  |
//   //  |
//   //  | (can read ~75MB/s (or maybe only ~50MB/s))
//   //   |     - (for 16 Burst Length with 64 Latency?)
//   //  \-> Splitter/Returner FPGA <----------------------------\ 
//   //     |                                                    |
//   //     |                                                    |
//   //     \-> Graph Construction                               |
//   //             |                                            |
//   //             |            (time estimate after pipeline)  |
//   //             |                                            |
//   //             \/  (~1KB of data / ~(122*5ns)) (~1.6GB/s)   |
//   //         Neural Network                                   |
//   //                \-----------------------------------------/



//   CREATE_M_1NODE_LOOP:
//   for(int node = 0; node < NHITS; node++){
//     #pragma HLS PIPELINE

//     data_t selector_src[NEDGES];
//     data_t selector_dst[NEDGES];
//     #pragma HLS ARRAY_PARTITION variable=selector_src complete dim=0
//     #pragma HLS ARRAY_PARTITION variable=selector_dst complete dim=0
//     for(int i = 0; i < NEDGES; i++){
//       #pragma HLS UNROLL
//       selector_src[i] = 0;
//       selector_dst[i] = 0;
//     }

//     CREATE_M_1EDGE_LOOP:
//     for(int edge = 0; edge < NEDGES; edge++){
//       #pragma HLS UNROLL
//       // #pragma HLS PIPELINE
//       // #pragma HLS PIPELINE off
//       // #pragma HLS unroll factor=1
//       // #pragma HLS unroll

//       i_data_t src_node = edge_index[edge*2];
//       i_data_t dst_node = edge_index[edge*2 + 1];
//       data_t edge_mult = e[edge];

//       if(src_node != i_data_t(-1)){
//         continue;
//       }

//       if(src_node == node){
//         selector_src[edge] = edge_mult;
//       }
//       if(dst_node == node){
//         selector_dst[edge] = edge_mult;
//       }
//     }

//     par_t aggregate;
//     dot_bound_T_select(inbound, selector_src, aggregate);
//     M[0*NHITS + node] = aggregate;
//     dot_bound_T_select(outbound, selector_dst, aggregate);
//     M[1*NHITS + node] = aggregate;
//   }


//   // CREATE_M_1EDGE_LOOP:
//   // for(int edge = 0; edge < NEDGES; edge++){
//   //   // #pragma HLS PIPELINE II=4 // was throwing errors until II=4 so TODO: try lowering? or something
//   //   #pragma HLS PIPELINE
    
//   //   // #pragma HLS PIPELINE off
//   //   // #pragma HLS unroll factor=1
//   //   // #pragma HLS unroll

//   //   par_t Hsrc = inbound[edge];
//   //   par_t Hdst = outbound[edge];

//   //   i_data_t src_node = edge_index[edge*2];
//   //   i_data_t dst_node = edge_index[edge*2 + 1];
//   //   data_t edge_mult = e[edge];

//   //   if(src_node != i_data_t(-1)){
//   //     continue;
//   //   }

//   //   // int onehot_src[NHITS];
//   //   // for(int i = 0; i < NHITS; i++){
//   //   //   onehot_src[i] = 0;
//   //   // }
//   //   // onehot_src[src_node] = 1;

//   //   // int onehot_dst[NHITS];
//   //   // for(int i = 0; i < NHITS; i++){
//   //   //   onehot_dst[i] = 0;
//   //   // }
//   //   // onehot_dst[dst_node] = 1;

//   //   // par_t tmp_Hs = dot(H, onehot_src);
//   //   // par_t tmp_Hd = dot(H, onehot_dst);
    
//   //   // TODO: figure out some matrix operation to improve performance
//   //   // write to temp array, then sum together? use hls::vector?
//   //   par_t tmp_MD = M[0*NHITS + dst_node];
//   //   par_t tmp_MS = M[1*NHITS + src_node];
//   //   par_t tmp_Hs = H[src_node];
//   //   par_t tmp_Hd = H[dst_node];

//   //   // par_t tmp1 = tmp_MD + (tmp_Hs * edge_mult);
//   //   // par_t tmp2 = tmp_MS + (tmp_Hd * edge_mult);
//   //   // M[dst_node][0] = tmp1;
//   //   // M[src_node][1] = tmp2;

//   //   M[0*NHITS + dst_node] = tmp_MD + (tmp_Hs * edge_mult);
//   //   M[1*NHITS + src_node] = tmp_MS + (tmp_Hd * edge_mult);
//   // }

// // CREATE_M_1EDGE_LOOP_FIRST:
// //   for(int edge = 0; edge < NEDGES; edge++){
// //     #pragma HLS PIPELINE off
// //     #pragma HLS unroll factor=1
// //     // #pragma HLS unroll

// //     i_data_t src_node = edge_index[edge*2];
// //     i_data_t dst_node = edge_index[edge*2 + 1];
// //     data_t edge_mult = e[edge];

// //     if(src_node != i_data_t(-1)){
// //       continue;
// //     }

// //     par_t tmp_MD = M[dst_node][0];
// //     par_t tmp_MS = M[src_node][1];
// //     M[dst_node][0] = tmp_MD + (H[src_node] * edge_mult);
// //     M[src_node][1] = tmp_MS + (H[dst_node] * edge_mult);
// //   }

// // CREATE_M_1EDGE_LOOP_SECOND:
// //   for(int edge = 0; edge < NEDGES; edge++){
// //     // #pragma HLS PIPELINE
// //     #pragma HLS unroll factor=1
// //     // #pragma HLS unroll

// //     i_data_t src_node = edge_index[edge*2];
// //     i_data_t dst_node = edge_index[edge*2 + 1];
// //     data_t edge_mult = e[edge];

// //     if(src_node != i_data_t(-1)){
// //       continue;
// //     }

// //     par_t tmp_MD = M[dst_node][0];
// //     par_t tmp_MS = M[src_node][1];
// //     M[dst_node][0] = tmp_MD + (H[src_node] * edge_mult);
// //     M[src_node][1] = tmp_MS + (H[dst_node] * edge_mult);
// //   }

// // CREATE_M_3NODE_LOOP:
// //   for(int node = 0; node < NHITS; node++){
// //     #pragma HLS PIPELINE
// //     // #pragma HLS unroll factor=1
// //     // #pragma HLS unroll

// //     for(int par = 0; par < NPARHID; par++){
// //       // #pragma HLS unroll factor=1
// //       #pragma HLS unroll
// //       M[2*NHITS + node][par] = H[node][par];
// //     }
// //   }
// }


// void node_runner(par_t M[3 * NHITS], par_t H[NHITS], par_t H_out[NHITS]){
// void node_runner(par_t inbound[NEDGES], par_t outbound[NEDGES], par_t H[NHITS], par_t H_out[NHITS]){
// void node_runner(par_t M[2 * NHITS], par_t H[NHITS], par_t H_out[NHITS]){
void node_runner(i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t inbound[NEDGES], par_t outbound[NEDGES], par_t H[NHITS], par_t H_out[NHITS]){
  // #pragma HLS PIPELINE
  node_net::input_t in1[N_INPUT_1_1];
  node_net::result_t out1[N_LAYER_8];
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

    par_t sum_in;
    par_t sum_out;

    for(int edge = 0; edge < NEDGES; edge++){
      i_data_t src = edge_index[2*edge];
      i_data_t dst = edge_index[2*edge+1];

      if(src == -1){
        break;
      }

      data_t e_mult = e[edge];
      par_t in = inbound[edge];
      par_t out = outbound[edge];

      if(src == i){
        sum_out = sum_out + out;
      }
      if(dst == i){
        sum_in = sum_in + in;
      }

    }

    AGGREGATE_TO_IN1:
    for(int j = 0; j < NPARHID; j++){
      #pragma HLS unroll
      in1[j]           = sum_out[j];
      in1[j+NPARHID]   = sum_in[j];
      in1[j+NPARHID*2] = H[i][j];
    }

    node_network_s(in1, out1);

    for(int j = 0; j < N_LAYER_8; j++){
      #pragma HLS unroll
      H_out[i][j] = out1[j];
    }
    for(int j = 0; j < NPARAMS; j++){
      #pragma HLS unroll
      H_out[i][j + NHIDDEN] = in1[j + (NPARHID3 - NPARAMS)];
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
