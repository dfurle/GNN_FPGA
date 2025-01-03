#pragma once
#include "ap_fixed.h"
#include "ap_int.h"
#include "nnet_utils/nnet_types.h"
#include <cstddef>
#include <cstdio>

namespace input_net{

// hls-fpga-machine-learning insert numbers
#define N_INPUT_1_1 3
#define N_LAYER_2 8
#define N_LAYER_2 8
#define N_LAYER_4 8
#define N_LAYER_4 8

// hls-fpga-machine-learning insert layer-precision
typedef ap_fixed<16,6> input_t;
typedef ap_fixed<16,6> model_default_t;
typedef ap_fixed<16,6> layer2_t;
typedef ap_uint<1> layer2_index;
typedef ap_fixed<16,6> layer3_t;
typedef ap_fixed<18,8> _0_block_1_table_t;
typedef ap_fixed<16,6> layer4_t;
typedef ap_uint<1> layer4_index;
typedef ap_fixed<16,6> result_t;
typedef ap_fixed<18,8> _1_block_1_table_t;

}
