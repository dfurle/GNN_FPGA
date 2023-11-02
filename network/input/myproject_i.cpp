#include <iostream>

#include "myproject_i.h"
#include "parameters_i.h"

namespace input_net{

void input_network_s(input_t dense_in_1_input[N_INPUT_1_1], result_t layer9_out[N_LAYER_8]);

}

#ifdef STREAM
void input_network(hls::stream<X_t>& X_stream, hls::stream<H_t>& H_stream){
#endif
#ifdef VECTOR
void input_network(X_t& X, H_t& H){
#endif
#ifdef ARRAY
void input_network(data_t X[NHITS * NPARAMS], data_t H[NHITS * NPARHID]){
#endif
  // #pragma HLS PIPELINE

  input_net::input_t in1[N_INPUT_1_1];
  input_net::result_t out1[N_LAYER_8];
  #pragma HLS ARRAY_PARTITION variable=in1
  #pragma HLS ARRAY_PARTITION variable=out1

  #ifdef STREAM
  X_t X;
  H_t H;
  X_stream >> X;
  #endif
INPUT_HIT_LOOP:
  for(int i = 0; i < NHITS; i++){
    #pragma HLS unroll factor=2

    // in1 = (input_t*) X[i].begin();

    for(int j = 0; j < N_INPUT_1_1; j++){
      #pragma HLS unroll
      #if defined(STREAM) || defined(VECTOR)
      in1[j] = X[i][j];
      #endif
      #ifdef ARRAY
      in1[j] = X[i*N_INPUT_1_1 + j];
      #endif
    }

    input_net::input_network_s(in1, out1);

    for(int j = 0; j < N_LAYER_8; j++){
      #pragma HLS unroll
      #if defined(STREAM) || defined(VECTOR)
      H[i][j] = out1[j];
      #endif
      #ifdef ARRAY
      H[i*(N_LAYER_8+N_INPUT_1_1) + j] = out1[j];
      #endif
    }
    for(int j = 0; j < N_INPUT_1_1; j++){
      #pragma HLS unroll
      #if defined(STREAM) || defined(VECTOR)
      H[i][j + N_LAYER_8] = in1[j];
      #endif
      #ifdef ARRAY
      H[i*(N_LAYER_8+N_INPUT_1_1) + j + N_LAYER_8] = in1[j];
      #endif
    }
  }

  #ifdef STREAM
  H_stream << H;
  #endif

}

namespace input_net{

void input_network_s(input_t in[N_INPUT_1_1], result_t out[N_LAYER_8]) {
  // hls-fpga-machine-learning insert IO
  #pragma HLS ARRAY_RESHAPE variable=in complete dim=0
  #pragma HLS ARRAY_PARTITION variable=out complete dim=0
  #pragma HLS INTERFACE ap_vld port=in,out 
  #pragma HLS PIPELINE 


  // hls-fpga-machine-learning insert layers

  layer2_t layer2_out[N_LAYER_2];
  #pragma HLS ARRAY_PARTITION variable=layer2_out complete dim=0
  nnet::dense<input_t, layer2_t, config2>(in, layer2_out, w1, b1); // dense_in_1

  layer3_t layer3_out[N_LAYER_2];
  #pragma HLS ARRAY_PARTITION variable=layer3_out complete dim=0
  nnet::relu<layer2_t, layer3_t, relu_config3>(layer2_out, layer3_out); // dense_in_1_relu

  layer4_t layer4_out[N_LAYER_4];
  #pragma HLS ARRAY_PARTITION variable=layer4_out complete dim=0
  nnet::dense<layer3_t, layer4_t, config4>(layer3_out, layer4_out, w2, b2); // dense_in_2

  layer5_t layer5_out[N_LAYER_4];
  #pragma HLS ARRAY_PARTITION variable=layer5_out complete dim=0
  nnet::relu<layer4_t, layer5_t, relu_config5>(layer4_out, layer5_out); // dense_in_2_relu

  layer6_t layer6_out[N_LAYER_6];
  #pragma HLS ARRAY_PARTITION variable=layer6_out complete dim=0
  nnet::dense<layer5_t, layer6_t, config6>(layer5_out, layer6_out, w3, b3); // dense_in_3

  layer7_t layer7_out[N_LAYER_6];
  #pragma HLS ARRAY_PARTITION variable=layer7_out complete dim=0
  nnet::relu<layer6_t, layer7_t, relu_config7>(layer6_out, layer7_out); // dense_in_3_relu

  layer8_t layer8_out[N_LAYER_8];
  #pragma HLS ARRAY_PARTITION variable=layer8_out complete dim=0
  nnet::dense<layer7_t, layer8_t, config8>(layer7_out, layer8_out, w4, b4); // dense_in_out

  nnet::relu<layer8_t, result_t, relu_config9>(layer8_out, out); // dense_in_out_relu

}
}