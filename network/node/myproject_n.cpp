#include <iostream>

#include "myproject_n.h"
#include "parameters_n.h"


namespace node_net{

void node_runner(data_t B[NEDGES * NPARHID2], data_t e[NEDGES]);
void node_network_s(input_t dense_node_1_input[N_INPUT_1_1], result_t layer11_out[N_LAYER_8]);

}

// bo = tf.matmul(tf.transpose(Ro, perm=[1,0]), X)
// bi = tf.matmul(tf.transpose(Ri, perm=[1,0]), X)
// Rwo = Ro * e[None]
// Rwi = Ri * e[None]
// mi = tf.matmul(Rwi, bo)
// mo = tf.matmul(Rwo, bi)
// M = tf.concat([mi, mo, X], axis=1)
// return self.node_network(M)

#ifdef STREAM
void node_network(hls::stream<H_t>& Hin_stream, hls::stream<R_t>& Ro_stream, hls::stream<R_t>& Ri_stream, hls::stream<e_t>& e_stream, hls::stream<H_t>& Hout_stream){
#endif
#ifdef VECTOR
void node_network(H_t& H, R_t& Ro, R_t& Ri, e_t& e, H_t& Hout){
#endif
#ifdef ARRAY
void node_network(data_t H[NHITS * NPARHID], R_data_t Ro[NHITS * NEDGES], R_data_t Ri[NHITS * NEDGES], data_t e[NEDGES], data_t Hout[NHITS * NPARHID]){
#endif

  #ifdef DISABLE_NODE

  #else

  #ifdef STREAM
  H_t H, Hout;
  R_t Ro, Ri;
  e_t e;

  Hin_stream >> H;
  Ro_stream >> Ro;
  Ri_stream >> Ri;
  e_stream >> e;
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

  data_t mo[NHITS * NPARHID];
  data_t mi[NHITS * NPARHID];

  for(int i = 0; i < NHITS; i++){
    // #pragma HLS unroll
    for(int j = 0; j < NPARHID; j++){
      // #pragma HLS unroll
      mo[i*NPARHID + j] = 0;
      mi[i*NPARHID + j] = 0;
      for(int k = 0; k < NEDGES; k++){
        // #pragma HLS unroll
        #if defined(STREAM) || defined(VECTOR)
        mo[i*NPARHID + j] += Ro[i][k] * e[k] * bo[k*NPARHID + j];
        mi[i*NPARHID + j] += Ri[i][k] * e[k] * bi[k*NPARHID + j];
        #endif
        #ifdef ARRAY
        mo[i*NPARHID + j] += Ro[i*NEDGES + k] * e[k] * bo[k*NPARHID + j];
        mi[i*NPARHID + j] += Ri[i*NEDGES + k] * e[k] * bi[k*NPARHID + j];
        #endif
      }
    }
  }

  data_t M[NHITS * NPARHID3];

  for(int i = 0; i < NHITS; i++){
    // #pragma HLS unroll
    for(int j = 0; j < NPARHID; j++){
      // #pragma HLS unroll
      M[i*NPARHID3 + j] = mi[i*NPARHID + j];
      M[i*NPARHID3 + j + NPARHID] = mo[i*NPARHID + j];
      #if defined(STREAM) || defined(VECTOR)
      M[i*NPARHID + j + NPARHID2] = H[i][j];
      #endif
      #ifdef ARRAY
      M[i*NPARHID3 + j + NPARHID2] = H[i*NPARHID + j];
      #endif
    }
  }

  #if defined(STREAM) || defined(VECTOR)
  node_net::node_runner(M, Hout.begin()->begin());
  #endif
  #ifdef ARRAY
  node_net::node_runner(M, Hout);
  #endif

  #ifdef STREAM
  Hout_stream << Hout;
  #endif

  #endif
}

namespace node_net{

void node_runner(data_t M[NHITS * NPARHID3], data_t H[NHITS * NPARHID]){
  node_net::input_t in1[N_INPUT_1_1];
  node_net::result_t out1[N_LAYER_8];

  for(int i = 0; i < NHITS; i++){
    // #pragma HLS unroll
    for(int j = 0; j < N_INPUT_1_1; j++){
      // #pragma HLS unroll
      in1[j] = M[i*N_INPUT_1_1 + j];
    }

    node_network_s(in1, out1);

    for(int j = 0; j < N_LAYER_8; j++){
      // #pragma HLS unroll
      H[i*NPARHID + j] = out1[j];
    }
    for(int j = 0; j < NPARAMS; j++){
      // #pragma HLS unroll
      H[i*NPARHID + j + NHIDDEN] = in1[j + (NPARHID3 - NPARAMS)];
    }
  }
}




void node_network_s(input_t in1[N_INPUT_1_1], result_t out1[N_LAYER_8]) {
  // hls-fpga-machine-learning insert IO
  #pragma HLS ARRAY_RESHAPE variable=in1 complete dim=0
  #pragma HLS ARRAY_PARTITION variable=out1 complete dim=0
  #pragma HLS INTERFACE ap_vld port=in1, out1 
  #pragma HLS PIPELINE 


  layer2_t layer2_out[N_LAYER_2];
  #pragma HLS ARRAY_PARTITION variable=layer2_out complete dim=0
  nnet::dense<input_t, layer2_t, config2>(in1, layer2_out, w1, b1); // dense_node_1

  layer3_t layer3_out[N_LAYER_2];
  #pragma HLS ARRAY_PARTITION variable=layer3_out complete dim=0
  nnet::relu<layer2_t, layer3_t, relu_config3>(layer2_out, layer3_out); // dense_node_1_relu

  layer4_t layer4_out[N_LAYER_4];
  #pragma HLS ARRAY_PARTITION variable=layer4_out complete dim=0
  nnet::dense<layer3_t, layer4_t, config4>(layer3_out, layer4_out, w2, b2); // dense_node_2

  layer5_t layer5_out[N_LAYER_4];
  #pragma HLS ARRAY_PARTITION variable=layer5_out complete dim=0
  nnet::relu<layer4_t, layer5_t, relu_config5>(layer4_out, layer5_out); // dense_node_2_relu

  layer6_t layer6_out[N_LAYER_6];
  #pragma HLS ARRAY_PARTITION variable=layer6_out complete dim=0
  nnet::dense<layer5_t, layer6_t, config6>(layer5_out, layer6_out, w3, b3); // dense_node_3

  layer7_t layer7_out[N_LAYER_6];
  #pragma HLS ARRAY_PARTITION variable=layer7_out complete dim=0
  nnet::relu<layer6_t, layer7_t, relu_config7>(layer6_out, layer7_out); // dense_node_3_relu

  layer8_t layer8_out[N_LAYER_8];
  #pragma HLS ARRAY_PARTITION variable=layer8_out complete dim=0
  nnet::dense<layer7_t, layer8_t, config8>(layer7_out, layer8_out, w4, b4); // dense_node_out

  nnet::relu<layer8_t, result_t, relu_config11>(layer8_out, out1); // dense_node_out_relu

}

}
