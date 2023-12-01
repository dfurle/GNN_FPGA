// #include <iostream>

#include "myproject_e.h"
#include "parameters_e.h"


size_t access(int x, int y, int dim2){
  return x*dim2 + y;
}

namespace edge_net{

void edge_runner(data_t B[NEDGES * NPARHID2], data_t e[NEDGES]);
void edge_network_s(input_t in1[N_INPUT_1_1], result_t out1[N_LAYER_8]);

void createB(data_t H[NHITS * NPARHID], i_data_t edge_index[NEDGES * 2], data_t B[NEDGES * NPARHID2]);

}


#ifdef STREAM
void edge_network(hls::stream<H_t>& H_stream, hls::stream<R_t>& Ro_stream, hls::stream<R_t>& Ri_stream, hls::stream<e_t>& e_stream){
#endif
#ifdef VECTOR
void edge_network(H_t& H, R_t& Ro, R_t& Ri, e_t& e){
#endif
#ifdef ARRAY
void edge_network(data_t H[NHITS * NPARHID], i_data_t edge_index[NEDGES * 2], data_t e[NEDGES], int valid_edges){
#endif
  // #pragma HLS PIPELINE

  #ifdef DISABLE_EDGE

  #else

  #ifdef STREAM
  H_t H;
  R_t Ro, Ri;
  e_t e;

  H_stream >> H;
  Ro_stream >> Ro;
  Ri_stream >> Ri;
  #endif

  data_t B[NEDGES * NPARHID2];
  // #pragma HLS ARRAY_PARTITION variable=B block factor=NEDGES
  edge_net::createB(H, edge_index, B);

  #if defined(STREAM) || defined(VECTOR)
  edge_net::edge_runner(B, e.begin());
  #endif
  #ifdef ARRAY
  edge_net::edge_runner(B, e);
  #endif

  for(int i = valid_edges; i < NEDGES; i++){
    e[i] = data_t(0.0);
  }

  #ifdef STREAM
  e_stream << e;
  #endif

  #endif
}

namespace edge_net{


// B = torch.cat([X[edge_index[:, 0]], X[edge_index[:, 1]]], dim=1)
void createB(data_t H[NHITS * NPARHID], i_data_t edge_index[NEDGES * 2], data_t B[NEDGES * NPARHID2]){

CREATE_B_LOOP:
  for(int edge = 0; edge < NEDGES; edge++){
    #pragma HLS unroll factor=1
    // #pragma HLS unroll factor=10
    // #pragma HLS PIPELINE
    i_data_t src_node = edge_index[edge*2];
    i_data_t dst_node = edge_index[edge*2 + 1];

    if(src_node == i_data_t(-1)){
      for(int par = 0; par < NPARHID; par++){
        #pragma HLS unroll
        B[edge*NPARHID2 + par]           = 0;
        B[edge*NPARHID2 + NPARHID + par] = 0;
      }
      continue;
    }

    // memcpy(&B[edge*NPARHID2],           &H[src_node*NPARHID], NPARHID);
    // memcpy(&B[edge*NPARHID2 + NPARHID], &H[dst_node*NPARHID], NPARHID);

    // else, if memcpy doesnt work
    for(int par = 0; par < NPARHID; par++){
      // #pragma HLS unroll factor=1
      #pragma HLS unroll
      B[edge*NPARHID2 + par]           = H[src_node*NPARHID + par];
      B[edge*NPARHID2 + NPARHID + par] = H[dst_node*NPARHID + par];
    }
  }
}


void edge_runner(data_t B[NEDGES * NPARHID2], data_t e[NEDGES]){
  // #pragma HLS PIPELINE
  edge_net::input_t in1[N_INPUT_1_1];
  edge_net::result_t out1[N_LAYER_8];

EDGE_R_EDGE_LOOP:
  for(int i = 0; i < NEDGES; i++){
    #pragma HLS unroll factor=2

    for(int j = 0; j < N_INPUT_1_1; j++){
      #pragma HLS unroll
      in1[j] = B[i*N_INPUT_1_1 + j];
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

