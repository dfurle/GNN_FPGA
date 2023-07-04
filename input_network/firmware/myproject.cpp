#include <iostream>

#include "myproject.h"
#include "parameters.h"

namespace input_net{

void input_network_s(input_t dense_in_1_input[N_INPUT_1_1], result_t layer9_out[N_LAYER_8]);

}

#ifdef STREAM
void input_network(hls::stream<X_t>& X_stream, hls::stream<H_t>& H_stream){
#endif
#ifdef VECTOR
void input_network(X_t X, H_t H){
#endif
  input_t in1[N_INPUT_1_1];
  result_t out1[N_LAYER_8];

  X_t X;
  H_t H;
  X_stream >> X;

  for(int i = 0; i < NHITS; i++){
    #pragma HLS unroll factor=NHITS

    // in1 = (input_t*) X[i].begin();

    for(int j = 0; j < N_INPUT_1_1; j++){
      #pragma HLS unroll factor=N_INPUT_1_1
      in1[j] = X[i][j];
    }

    input_net::input_network_s(in1, out1);

    for(int j = 0; j < N_LAYER_8; j++){
      #pragma HLS unroll factor=N_LAYER_8
      H[i][j] = out1[j];
    }
    for(int j = 0; j < N_INPUT_1_1; j++){
      #pragma HLS unroll factor=N_INPUT_1_1
      H[i][j + N_LAYER_8] = in1[j];
    }
  }

  H_stream << H;
}

namespace input_net{

void input_network_s(input_t dense_in_1_input[N_INPUT_1_1], result_t layer9_out[N_LAYER_8]) {

    // hls-fpga-machine-learning insert IO
    #pragma HLS ARRAY_RESHAPE variable=dense_in_1_input complete dim=0
    #pragma HLS ARRAY_PARTITION variable=layer9_out complete dim=0
    #pragma HLS INTERFACE ap_vld port=dense_in_1_input,layer9_out 
    #pragma HLS PIPELINE 

// #ifndef __SYNTHESIS__
//     static bool loaded_weights = false;
//     if (!loaded_weights) {
//         // hls-fpga-machine-learning insert load weights
//         nnet::load_weights_from_txt<dense_in_1_weight_t, 16>(w2, "w2.txt");
//         nnet::load_weights_from_txt<dense_in_1_bias_t, 8>(b2, "b2.txt");
//         nnet::load_weights_from_txt<dense_in_2_weight_t, 64>(w4, "w4.txt");
//         nnet::load_weights_from_txt<dense_in_2_bias_t, 8>(b4, "b4.txt");
//         nnet::load_weights_from_txt<dense_in_3_weight_t, 64>(w6, "w6.txt");
//         nnet::load_weights_from_txt<dense_in_3_bias_t, 8>(b6, "b6.txt");
//         nnet::load_weights_from_txt<dense_in_out_weight_t, 64>(w8, "w8.txt");
//         nnet::load_weights_from_txt<dense_in_out_bias_t, 8>(b8, "b8.txt");
//         loaded_weights = true;
//     }
// #endif

    // ****************************************
    // NETWORK INSTANTIATION
    // ****************************************

    // hls-fpga-machine-learning insert layers

    layer2_t layer2_out[N_LAYER_2];
    #pragma HLS ARRAY_PARTITION variable=layer2_out complete dim=0
    nnet::dense<input_t, layer2_t, config2>(dense_in_1_input, layer2_out, w2, b2); // dense_in_1

    layer3_t layer3_out[N_LAYER_2];
    #pragma HLS ARRAY_PARTITION variable=layer3_out complete dim=0
    nnet::relu<layer2_t, layer3_t, relu_config3>(layer2_out, layer3_out); // dense_in_1_relu

    layer4_t layer4_out[N_LAYER_4];
    #pragma HLS ARRAY_PARTITION variable=layer4_out complete dim=0
    nnet::dense<layer3_t, layer4_t, config4>(layer3_out, layer4_out, w4, b4); // dense_in_2

    layer5_t layer5_out[N_LAYER_4];
    #pragma HLS ARRAY_PARTITION variable=layer5_out complete dim=0
    nnet::relu<layer4_t, layer5_t, relu_config5>(layer4_out, layer5_out); // dense_in_2_relu

    layer6_t layer6_out[N_LAYER_6];
    #pragma HLS ARRAY_PARTITION variable=layer6_out complete dim=0
    nnet::dense<layer5_t, layer6_t, config6>(layer5_out, layer6_out, w6, b6); // dense_in_3

    layer7_t layer7_out[N_LAYER_6];
    #pragma HLS ARRAY_PARTITION variable=layer7_out complete dim=0
    nnet::relu<layer6_t, layer7_t, relu_config7>(layer6_out, layer7_out); // dense_in_3_relu

    layer8_t layer8_out[N_LAYER_8];
    #pragma HLS ARRAY_PARTITION variable=layer8_out complete dim=0
    nnet::dense<layer7_t, layer8_t, config8>(layer7_out, layer8_out, w8, b8); // dense_in_out

    nnet::relu<layer8_t, result_t, relu_config9>(layer8_out, layer9_out); // dense_in_out_relu

}
}