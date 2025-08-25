#include <iostream>

#include "myproject_i.h"
#include "parameters_i.h"

namespace input_net{
  void input_network_s(input_t input_1[N_INPUT_1_1], result_t layer7_out[N_LAYER_6]);
}

// Insert Here

// End

namespace input_net{

void input_network_s(input_t input_1[N_INPUT_1_1], result_t layer7_out[N_LAYER_6]) {

    // hls-fpga-machine-learning insert IO
    #pragma HLS ARRAY_RESHAPE variable=input_1 complete dim=0
    #pragma HLS ARRAY_PARTITION variable=layer7_out complete dim=0
    #pragma HLS INTERFACE ap_vld port=input_1,layer7_out 
    #pragma HLS PIPELINE 


    // ****************************************
    // NETWORK INSTANTIATION
    // ****************************************

    // hls-fpga-machine-learning insert layers

    layer2_t layer2_out[N_LAYER_2];
    #pragma HLS ARRAY_PARTITION variable=layer2_out complete dim=0
    nnet::dense<input_t, layer2_t, config2>(input_1, layer2_out, w2, b2); // _0_block_0

    layer3_t layer3_out[N_LAYER_2];
    #pragma HLS ARRAY_PARTITION variable=layer3_out complete dim=0
    nnet::relu<layer2_t, layer3_t, ReLU_config3>(layer2_out, layer3_out); // _0_block_1

    layer4_t layer4_out[N_LAYER_4];
    #pragma HLS ARRAY_PARTITION variable=layer4_out complete dim=0
    nnet::dense<layer3_t, layer4_t, config4>(layer3_out, layer4_out, w4, b4); // _1_block_0

    layer5_t layer5_out[N_LAYER_4];
    #pragma HLS ARRAY_PARTITION variable=layer5_out complete dim=0
    nnet::relu<layer4_t, layer5_t, ReLU_config5>(layer4_out, layer5_out); // _1_block_1

    layer6_t layer6_out[N_LAYER_6];
    #pragma HLS ARRAY_PARTITION variable=layer6_out complete dim=0
    nnet::dense<layer5_t, layer6_t, config6>(layer5_out, layer6_out, w6, b6); // _2_block_0

    nnet::relu<layer6_t, result_t, ReLU_config7>(layer6_out, layer7_out); // _2_block_1

}

}
