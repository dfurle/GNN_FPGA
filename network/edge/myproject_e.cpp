// #include <iostream>

#include "myproject_e.h"
#include "parameters_e.h"


size_t access(int x, int y, int dim2){
  return x*dim2 + y;
}

namespace edge_net{

void edge_runner(data_t B[NEDGES * NPARHID2], data_t e[NEDGES]);
void edge_network_s(input_t in1[N_INPUT_1_1], result_t out1[N_LAYER_8]);

}

#ifdef STREAM
void edge_network(hls::stream<H_t>& H_stream, hls::stream<R_t>& Ro_stream, hls::stream<R_t>& Ri_stream, hls::stream<e_t>& e_stream){
#endif
#ifdef VECTOR
void edge_network(H_t& H, R_t& Ro, R_t& Ri, e_t& e){
#endif
#ifdef ARRAY
void edge_network(data_t H[NHITS * NPARHID], R_data_t Ro[NHITS * NEDGES], R_data_t Ri[NHITS * NEDGES], data_t e[NEDGES]){
#endif

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

  data_t bo[NEDGES * NPARHID];
  data_t bi[NEDGES * NPARHID];

  for(int i = 0; i < NEDGES; i++){
    // #pragma HLS unroll
    for(int j = 0; j < NPARHID; j++){
      // #pragma HLS unroll
      bo[i*NPARHID + j] = 0;
      bi[i*NPARHID + j] = 0;
      for(int k = 0; k < NHITS; k++){
        // #pragma HLS unroll
        #if defined(STREAM) || defined(VECTOR)
        bo[i*NPARHID + j] += Ro[k][i] * H[k][j];
        bi[i*NPARHID + j] += Ri[k][i] * H[k][j];
        #endif
        #ifdef ARRAY
        bo[i*NPARHID + j] += Ro[k*NEDGES + i] * H[k*NPARHID + j];
        bi[i*NPARHID + j] += Ri[k*NEDGES + i] * H[k*NPARHID + j];
        #endif
      }
    }
  }

  data_t B[NEDGES * NPARHID2];

  for(int i = 0; i < NEDGES; i++){
    // #pragma HLS unroll
    for(int j = 0; j < NPARHID; j++){
      // #pragma HLS unroll
      B[i*NPARHID2 + j] = bo[i*NPARHID + j];
    }
    for(int j = 0; j < NPARHID; j++){
      // #pragma HLS unroll
      B[i*NPARHID2 + j + NPARHID] = bi[i*NPARHID + j];
    }
  }

  #if defined(STREAM) || defined(VECTOR)
  edge_net::edge_runner(B, e.begin());
  #endif
  #ifdef ARRAY
  edge_net::edge_runner(B, e);
  #endif

  #ifdef STREAM
  e_stream << e;
  #endif

  #endif
}

namespace edge_net{

void edge_runner(data_t B[NEDGES * NPARHID2], data_t e[NEDGES]){
  edge_net::input_t in1[N_INPUT_1_1];
  edge_net::result_t out1[N_LAYER_8];

  for(int i = 0; i < NEDGES; i++){
    // #pragma HLS unroll

    for(int j = 0; j < N_INPUT_1_1; j++){
      // #pragma HLS unroll
      in1[j] = B[i*N_INPUT_1_1 + j];
    }

    edge_network_s(in1, out1);

    for(int j = 0; j < N_LAYER_8; j++){
      // #pragma HLS unroll
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

