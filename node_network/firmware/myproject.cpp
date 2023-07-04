#include <iostream>

#include "myproject.h"
#include "parameters.h"


namespace node_net{

void node_runner(data_t B[NEDGES * NPARHID2], data_t e[NEDGES]);
void node_network_s(input_t dense_node_1_input[N_INPUT_1_1], result_t layer11_out[N_LAYER_10]);

}

// bo = tf.matmul(tf.transpose(Ro, perm=[1,0]), X)
// bi = tf.matmul(tf.transpose(Ri, perm=[1,0]), X)
// Rwo = Ro * e[None]
// Rwi = Ri * e[None]
// mi = tf.matmul(Rwi, bo)
// mo = tf.matmul(Rwo, bi)
// M = tf.concat([mi, mo, X], axis=1)
// return self.node_network(M)

void node_network(hls::stream<H_t>& Hin_stream, hls::stream<R_t>& Ro_stream, hls::stream<R_t>& Ri_stream, hls::stream<e_t>& e_stream, hls::stream<H_t>& Hout_stream){

  H_t H, Hout;
  R_t Ro, Ri;
  e_t e;

  Hin_stream >> H;
  Ro_stream >> Ro;
  Ri_stream >> Ri;
  e_stream >> e;

  data_t bo[NEDGES * NPARHID];
  data_t bi[NEDGES * NPARHID];

  for(int i = 0; i < NEDGES; i++){
    #pragma HLS unroll factor=NEDGES
    for(int j = 0; j < NPARHID; j++){
      #pragma HLS unroll factor=NPARHID
      bo[i*NPARHID + j] = 0;
      bi[i*NPARHID + j] = 0;
      for(int k = 0; k < NHITS; k++){
        #pragma HLS unroll factor=NHITS
        bo[i*NPARHID + j] += Ro[k][i] * H[k][j];
        bi[i*NPARHID + j] += Ri[k][i] * H[k][j];
      }
    }
  }

  data_t mo[NHITS * NPARHID];
  data_t mi[NHITS * NPARHID];

  for(int i = 0; i < NHITS; i++){
    #pragma HLS unroll factor=NHITS
    for(int j = 0; j < NPARHID; j++){
      #pragma HLS unroll factor=NPARHID
      mo[i*NPARHID + j] = 0;
      mi[i*NPARHID + j] = 0;
      for(int k = 0; k < NEDGES; k++){
        #pragma HLS unroll factor=NEDGES
        mo[i*NPARHID + j] += Ro[i][k] * e[k] * bo[k*NPARHID + j];
        mi[i*NPARHID + j] += Ri[i][k] * e[k] * bi[k*NPARHID + j];
      }
    }
  }

  data_t M[NHITS * NPARHID3];

  for(int i = 0; i < NHITS; i++){
    #pragma HLS unroll factor=NHITS
    for(int j = 0; j < NPARHID; j++){
      #pragma HLS unroll factor=NPARHID
      M[i*NPARHID + j] = mi[i*NPARHID + j];
      M[i*NPARHID + j + NPARHID] = mo[i*NPARHID + j];
      M[i*NPARHID + j + NPARHID2] = H[i][j];
    }
  }
  node_net::node_runner(M, Hout.begin()->begin());

  Hout_stream << Hout;
}

namespace node_net{

void node_runner(data_t M[NHITS * NPARHID3], data_t H[NHITS * NPARHID]){
  input_t in1[N_INPUT_1_1];
  result_t out1[N_LAYER_8];

  for(int i = 0; i < NHITS; i++){
    #pragma HLS unroll factor=NHITS

    for(int j = 0; j < N_INPUT_1_1; j++){
      #pragma HLS unroll factor=N_INPUT_1_1
      in1[j] = M[i*N_INPUT_1_1 + j];
    }

    node_network_s(in1, out1);

    for(int j = 0; j < N_LAYER_8; j++){
      #pragma HLS unroll factor=N_LAYER_8
      H[i*NPARHID + j] = out1[j];
    }
    for(int j = 0; j < NPARAMS; j++){
      #pragma HLS unroll factor=NPARAMS
      H[i*NPARHID + j + NHIDDEN] = in1[j + (NPARHID3 - NPARAMS)];
    }
  }
}




void node_network_s(input_t dense_node_1_input[N_INPUT_1_1], result_t layer11_out[N_LAYER_10]) {

    // hls-fpga-machine-learning insert IO
    #pragma HLS ARRAY_RESHAPE variable=dense_node_1_input complete dim=0
    #pragma HLS ARRAY_PARTITION variable=layer11_out complete dim=0
    #pragma HLS INTERFACE ap_vld port=dense_node_1_input,layer11_out 
    #pragma HLS PIPELINE 

// #ifndef __SYNTHESIS__
//     static bool loaded_weights = false;
//     if (!loaded_weights) {
//         // hls-fpga-machine-learning insert load weights
//         nnet::load_weights_from_txt<dense_node_1_weight_t, 240>(w2, "w2.txt");
//         nnet::load_weights_from_txt<dense_node_1_bias_t, 8>(b2, "b2.txt");
//         nnet::load_weights_from_txt<dense_node_2_weight_t, 64>(w4, "w4.txt");
//         nnet::load_weights_from_txt<dense_node_2_bias_t, 8>(b4, "b4.txt");
//         nnet::load_weights_from_txt<dense_node_3_weight_t, 64>(w6, "w6.txt");
//         nnet::load_weights_from_txt<dense_node_3_bias_t, 8>(b6, "b6.txt");
//         nnet::load_weights_from_txt<dense_node_4_weight_t, 64>(w8, "w8.txt");
//         nnet::load_weights_from_txt<dense_node_4_bias_t, 8>(b8, "b8.txt");
//         nnet::load_weights_from_txt<dense_node_out_weight_t, 64>(w10, "w10.txt");
//         nnet::load_weights_from_txt<dense_node_out_bias_t, 8>(b10, "b10.txt");
//         loaded_weights = true;
//     }
// #endif

    // ****************************************
    // NETWORK INSTANTIATION
    // ****************************************

    // hls-fpga-machine-learning insert layers

    layer2_t layer2_out[N_LAYER_2];
    #pragma HLS ARRAY_PARTITION variable=layer2_out complete dim=0
    nnet::dense<input_t, layer2_t, config2>(dense_node_1_input, layer2_out, w2, b2); // dense_node_1

    layer3_t layer3_out[N_LAYER_2];
    #pragma HLS ARRAY_PARTITION variable=layer3_out complete dim=0
    nnet::relu<layer2_t, layer3_t, relu_config3>(layer2_out, layer3_out); // dense_node_1_relu

    layer4_t layer4_out[N_LAYER_4];
    #pragma HLS ARRAY_PARTITION variable=layer4_out complete dim=0
    nnet::dense<layer3_t, layer4_t, config4>(layer3_out, layer4_out, w4, b4); // dense_node_2

    layer5_t layer5_out[N_LAYER_4];
    #pragma HLS ARRAY_PARTITION variable=layer5_out complete dim=0
    nnet::relu<layer4_t, layer5_t, relu_config5>(layer4_out, layer5_out); // dense_node_2_relu

    layer6_t layer6_out[N_LAYER_6];
    #pragma HLS ARRAY_PARTITION variable=layer6_out complete dim=0
    nnet::dense<layer5_t, layer6_t, config6>(layer5_out, layer6_out, w6, b6); // dense_node_3

    layer7_t layer7_out[N_LAYER_6];
    #pragma HLS ARRAY_PARTITION variable=layer7_out complete dim=0
    nnet::relu<layer6_t, layer7_t, relu_config7>(layer6_out, layer7_out); // dense_node_3_relu

    layer8_t layer8_out[N_LAYER_8];
    #pragma HLS ARRAY_PARTITION variable=layer8_out complete dim=0
    nnet::dense<layer7_t, layer8_t, config8>(layer7_out, layer8_out, w8, b8); // dense_node_4

    layer9_t layer9_out[N_LAYER_8];
    #pragma HLS ARRAY_PARTITION variable=layer9_out complete dim=0
    nnet::relu<layer8_t, layer9_t, relu_config9>(layer8_out, layer9_out); // dense_node_4_relu

    layer10_t layer10_out[N_LAYER_10];
    #pragma HLS ARRAY_PARTITION variable=layer10_out complete dim=0
    nnet::dense<layer9_t, layer10_t, config10>(layer9_out, layer10_out, w10, b10); // dense_node_out

    nnet::relu<layer10_t, result_t, relu_config11>(layer10_out, layer11_out); // dense_node_out_relu

}

}
