// #include <iostream>

#include "myproject_e.h"
#include "parameters_e.h"

namespace edge_net{

// void edge_runner(par2_t B[NEDGES], data_t e[NEDGES]);
void edge_runner(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t inbound[NEDGES], par_t outbound[NEDGES]);
void edge_network_s(input_t in1[N_INPUT_1_1], result_t out1[N_LAYER_8]);
// void createB(par_t H[NHITS], i_data_t edge_index[NEDGES*2], par2_t B[NEDGES]);

}


void edge_network(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], int valid_edges, par_t inbound[NEDGES], par_t outbound[NEDGES]){
  // #pragma HLS PIPELINE

  // par2_t B[NEDGES];

  // edge_net::createB(H, edge_index, B);

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



// par_t dot(par_t* a, int* w) {
//   #pragma HLS PIPELINE
//   #pragma HLS ARRAY_PARTITION variable=a complete dim=0
//   #pragma HLS ARRAY_PARTITION variable=w complete dim=0
// 	par_t out;
//   #pragma HLS ARRAY_PARTITION variable=out complete dim=0
// 	for (int i = 0; i < NPARHID; i++) {
//     float sum = 0;
//     for (int k = 0; k < NHITS; k++) {
//       sum = sum + float(a[k][i] * w[k]);
//     }
//     out[i] = sum;
// 	}
// 	return out;
// }


// void createB(par_t H[NHITS], i_data_t edge_index[NEDGES*2], par2_t B[NEDGES]){
//   // #pragma HLS PIPELINE
//   // #pragma HLS INLINE off

// CREATE_B_LOOP:
//   for(int edge = 0; edge < NEDGES; edge++){
//     #pragma HLS PIPELINE
//     // #pragma HLS unroll
//     // #pragma HLS unroll factor=1
//     // #pragma HLS unroll factor=10
//     i_data_t src_node = edge_index[edge*2];
//     i_data_t dst_node = edge_index[edge*2 + 1];

//     if(src_node == i_data_t(-1)){
//       for(int par = 0; par < NPARHID; par++){
//         #pragma HLS unroll
//         B[edge][0][par] = 0;
//         B[edge][1][par] = 0;
//       }
//       continue;
//     }


//     int onehot_src[NHITS];
//     for(int i = 0; i < NHITS; i++){
//       onehot_src[i] = 0;
//     }
//     onehot_src[src_node] = 1;

//     int onehot_dst[NHITS];
//     for(int i = 0; i < NHITS; i++){
//       onehot_dst[i] = 0;
//     }
//     onehot_dst[dst_node] = 1;

//     par_t tmp1 = dot(H, onehot_src);
//     par_t tmp2 = dot(H, onehot_dst);


//     // possibly interleve?
//     // par_t tmp1 = H[src_node];
//     B[edge][0] = tmp1;
//     // par_t tmp2 = H[dst_node];
//     B[edge][1] = tmp2;
//   }
// }


void edge_runner(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t inbound[NEDGES], par_t outbound[NEDGES]){
  // #pragma HLS INLINE off
  // #pragma HLS PIPELINE
  edge_net::input_t in1[N_INPUT_1_1];
  edge_net::result_t out1[N_LAYER_8];

EDGE_R_EDGE_LOOP:
  for(int i = 0; i < NEDGES; i++){
    // #pragma HLS unroll factor=2

    // for(int j = 0; j < N_INPUT_1_1; j++){
    //   #pragma HLS unroll
    //   in1[j] = B[i][j/NPARHID][j%NPARHID];
    // }
    i_data_t src = edge_index[2*i];
    i_data_t dst = edge_index[2*i+1];

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

    for(int j = 0; j < N_LAYER_8; j++){
      #pragma HLS unroll
      e[i] = out1[j];
    }
  }
}


void edge_network_s(input_t in1[N_INPUT_1_1], result_t out1[N_LAYER_8]) {
  // hls-fpga-machine-learning insert IO
  #pragma HLS ARRAY_RESHAPE variable=in1 complete dim=0
  #pragma HLS ARRAY_PARTITION variable=out1 complete dim=0
  #pragma HLS INTERFACE ap_vld port=in1,out1 
  #pragma HLS PIPELINE 

  layer2_t layer2_out[N_LAYER_2];
  #pragma HLS ARRAY_PARTITION variable=layer2_out complete dim=0
  nnet::dense<input_t, layer2_t, config2>(in1, layer2_out, w1, b1); // dense_edge_1

  layer3_t layer3_out[N_LAYER_2];
  #pragma HLS ARRAY_PARTITION variable=layer3_out complete dim=0
  nnet::relu<layer2_t, layer3_t, relu_config3>(layer2_out, layer3_out); // dense_edge_1_relu

  layer4_t layer4_out[N_LAYER_4];
  #pragma HLS ARRAY_PARTITION variable=layer4_out complete dim=0
  nnet::dense<layer3_t, layer4_t, config4>(layer3_out, layer4_out, w2, b2); // dense_edge_2

  layer5_t layer5_out[N_LAYER_4];
  #pragma HLS ARRAY_PARTITION variable=layer5_out complete dim=0
  nnet::relu<layer4_t, layer5_t, relu_config5>(layer4_out, layer5_out); // dense_edge_2_relu

  layer6_t layer6_out[N_LAYER_6];
  #pragma HLS ARRAY_PARTITION variable=layer6_out complete dim=0
  nnet::dense<layer5_t, layer6_t, config6>(layer5_out, layer6_out, w3, b3); // dense_edge_3

  layer7_t layer7_out[N_LAYER_6];
  #pragma HLS ARRAY_PARTITION variable=layer7_out complete dim=0
  nnet::relu<layer6_t, layer7_t, relu_config7>(layer6_out, layer7_out); // dense_edge_3_relu

  layer8_t layer8_out[N_LAYER_8];
  #pragma HLS ARRAY_PARTITION variable=layer8_out complete dim=0
  nnet::dense<layer7_t, layer8_t, config8>(layer7_out, layer8_out, w4, b4); // dense_edge_out

  nnet::sigmoid<layer8_t, result_t, sigmoid_config9>(layer8_out, out1); // dense_edge_out_sigmoid
}

}

