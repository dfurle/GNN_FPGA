#ifndef DEFINES_H_NODE
#define DEFINES_H_NODE

#include "ap_fixed.h"
#include "ap_int.h"
#include "nnet_utils/nnet_types.h"
#include <cstddef>
#include <cstdio>

namespace node_net{

// hls-fpga-machine-learning insert numbers
#define N_INPUT_1_1 30
#define N_LAYER_2 8
#define N_LAYER_2 8
#define N_LAYER_4 8
#define N_LAYER_4 8
#define N_LAYER_6 8
#define N_LAYER_6 8
#define N_LAYER_8 8
#define N_LAYER_8 8

// hls-fpga-machine-learning insert layer-precision
typedef ap_fixed<16,6> input_t;
typedef ap_fixed<16,6> model_default_t;
typedef ap_fixed<16,6> layer2_t;
typedef ap_fixed<16,6> dense_node_1_weight_t;
typedef ap_fixed<16,6> dense_node_1_bias_t;
typedef ap_uint<1> layer2_index;
typedef ap_fixed<16,6> layer3_t;
typedef ap_fixed<18,8> dense_node_1_relu_table_t;
typedef ap_fixed<16,6> layer4_t;
typedef ap_fixed<16,6> dense_node_2_weight_t;
typedef ap_fixed<16,6> dense_node_2_bias_t;
typedef ap_uint<1> layer4_index;
typedef ap_fixed<16,6> layer5_t;
typedef ap_fixed<18,8> dense_node_2_relu_table_t;
typedef ap_fixed<16,6> layer6_t;
typedef ap_fixed<16,6> dense_node_3_weight_t;
typedef ap_fixed<16,6> dense_node_3_bias_t;
typedef ap_uint<1> layer6_index;
typedef ap_fixed<16,6> layer7_t;
typedef ap_fixed<18,8> dense_node_3_relu_table_t;
typedef ap_fixed<16,6> layer8_t;
typedef ap_fixed<16,6> dense_node_out_weight_t;
typedef ap_fixed<16,6> dense_node_out_bias_t;
typedef ap_uint<1> layer8_index;
typedef ap_fixed<16,6> result_t;
typedef ap_fixed<18,8> dense_node_out_relu_table_t;

}

#endif
