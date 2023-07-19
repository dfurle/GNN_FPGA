#ifndef PARAMETERS_H_EDGE
#define PARAMETERS_H_EDGE


#include "ap_fixed.h"
#include "ap_int.h"

#include "nnet_utils/nnet_code_gen.h"
#include "nnet_utils/nnet_helpers.h"
// hls-fpga-machine-learning insert includes
#include "nnet_utils/nnet_activation.h"
#include "nnet_utils/nnet_activation_stream.h"
#include "nnet_utils/nnet_dense.h"
#include "nnet_utils/nnet_dense_compressed.h"
#include "nnet_utils/nnet_dense_stream.h"

namespace edge_net{

// hls-fpga-machine-learning insert weights
#define __SYNTHESIS__
#include "weights_e/w2.h"
#include "weights_e/b2.h"
#include "weights_e/w4.h"
#include "weights_e/b4.h"
#include "weights_e/w6.h"
#include "weights_e/b6.h"
#include "weights_e/w8.h"
#include "weights_e/b8.h"
#undef __SYNTHESIS__

// hls-fpga-machine-learning insert layer-config
// dense_edge_1
struct config2 : nnet::dense_config {
    static const unsigned n_in = 20;
    static const unsigned n_out = 8;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::latency;
    static const unsigned reuse_factor = 1;
    static const unsigned n_zeros = 120;
    static const unsigned n_nonzeros = 40;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef dense_edge_1_bias_t bias_t;
    typedef dense_edge_1_weight_t weight_t;
    typedef layer2_index index_t;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_edge_1_relu
struct relu_config3 : nnet::activ_config {
    static const unsigned n_in = 8;
    static const unsigned table_size = 1024;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 1;
    typedef dense_edge_1_relu_table_t table_t;
};

// dense_edge_2
struct config4 : nnet::dense_config {
    static const unsigned n_in = 8;
    static const unsigned n_out = 8;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::latency;
    static const unsigned reuse_factor = 1;
    static const unsigned n_zeros = 48;
    static const unsigned n_nonzeros = 16;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef dense_edge_2_bias_t bias_t;
    typedef dense_edge_2_weight_t weight_t;
    typedef layer4_index index_t;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_edge_2_relu
struct relu_config5 : nnet::activ_config {
    static const unsigned n_in = 8;
    static const unsigned table_size = 1024;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 1;
    typedef dense_edge_2_relu_table_t table_t;
};

// dense_edge_3
struct config6 : nnet::dense_config {
    static const unsigned n_in = 8;
    static const unsigned n_out = 8;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::latency;
    static const unsigned reuse_factor = 1;
    static const unsigned n_zeros = 48;
    static const unsigned n_nonzeros = 16;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef dense_edge_3_bias_t bias_t;
    typedef dense_edge_3_weight_t weight_t;
    typedef layer6_index index_t;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_edge_3_relu
struct relu_config7 : nnet::activ_config {
    static const unsigned n_in = 8;
    static const unsigned table_size = 1024;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 1;
    typedef dense_edge_3_relu_table_t table_t;
};

// dense_edge_out
struct config8 : nnet::dense_config {
    static const unsigned n_in = 8;
    static const unsigned n_out = 1;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned strategy = nnet::latency;
    static const unsigned reuse_factor = 1;
    static const unsigned n_zeros = 6;
    static const unsigned n_nonzeros = 2;
    static const unsigned multiplier_limit = DIV_ROUNDUP(n_in * n_out, reuse_factor) - n_zeros / reuse_factor;
    static const bool store_weights_in_bram = false;
    typedef model_default_t accum_t;
    typedef dense_edge_out_bias_t bias_t;
    typedef dense_edge_out_weight_t weight_t;
    typedef layer8_index index_t;
    template<class x_T, class y_T>
    using product = nnet::product::mult<x_T, y_T>;
};

// dense_edge_out_sigmoid
struct sigmoid_config9 : nnet::activ_config {
    static const unsigned n_in = 1;
    static const unsigned table_size = 1024;
    static const unsigned io_type = nnet::io_parallel;
    static const unsigned reuse_factor = 1;
    typedef dense_edge_out_sigmoid_table_t table_t;
};

}

#endif
