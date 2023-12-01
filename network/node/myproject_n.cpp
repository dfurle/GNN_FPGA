#include <iostream>

#include "myproject_n.h"
#include "parameters_n.h"


namespace node_net{

void node_runner(data_t B[NEDGES * NPARHID2], data_t e[NEDGES]);
void node_network_s(input_t dense_node_1_input[N_INPUT_1_1], result_t layer11_out[N_LAYER_8]);

void createM(data_t H[NHITS * NPARHID], data_t e[NEDGES], i_data_t edge_index[NEDGES * 2], data_t M[NHITS * NPARHID3]);
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
void node_network(data_t H[NHITS * NPARHID], i_data_t edge_index[NEDGES * 2], data_t e[NEDGES], data_t Hout[NHITS * NPARHID]){
#endif
  // #pragma HLS PIPELINE

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

  data_t M[NHITS * NPARHID3];
  // #pragma HLS ARRAY_PARTITION variable=M block factor=NHITS
  node_net::createM(H, e, edge_index, M);

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


void createM(data_t H[NHITS * NPARHID], data_t e[NEDGES], i_data_t edge_index[NEDGES * 2], data_t M[NHITS * NPARHID3]){
  // #pragma HLS PIPELINE

  // src, dst = edge_index[:, 0], edge_index[:, 1]
  // src_features = X[src]
  // dst_features = X[dst]

  // # Compute incoming messages
  // src_aggregate = e.unsqueeze(1) * src_features
  // dst_aggregate = e.unsqueeze(1) * dst_features

  data_t SRC_AGGREGATE[NEDGES * NPARHID];
  // #pragma ARRAY_PARTITION variable=SRC_AGGREGATE cyclic
  data_t DST_AGGREGATE[NEDGES * NPARHID];
  // #pragma ARRAY_PARTITION variable=DST_AGGREGATE cyclic


  for(int node = 0; node < NHITS; node++){
    for(int par = 0; par < NPARHID3; par++){
      M[node* NPARHID3 + par] = 0;
    }
  }

CREATE_M_1EDGE_LOOP:
  for(int edge = 0; edge < NEDGES; edge++){
    // #pragma HLS PIPELINE
    // #pragma HLS unroll
    #pragma HLS unroll factor=1
    // #pragma HLS unroll factor=10
    i_data_t src_node = edge_index[edge*2];
    i_data_t dst_node = edge_index[edge*2 + 1];


    if(src_node == i_data_t(-1)){
      break;
    }


CREATE_M_2PAR_LOOP:
    for(int par = 0; par < NPARHID; par++){
      // #pragma HLS unroll factor=1
      #pragma HLS unroll
      int edge_index = edge*NPARHID + par;
      int src_index = src_node*NPARHID + par;
      int dst_index = dst_node*NPARHID + par;
      SRC_AGGREGATE[edge_index] = H[src_index] * e[edge];
      DST_AGGREGATE[edge_index] = H[dst_index] * e[edge];

      dst_index = dst_node*NPARHID3 + par;
      src_index = src_node*NPARHID3 + par + NPARHID;
      M[dst_index] += SRC_AGGREGATE[edge_index];
      M[src_index] += DST_AGGREGATE[edge_index];
    }
  }

  // # Aggregate incoming and outgoing messages
  // mi = torch.zeros(X.shape).to(self.devices)
  // mo = torch.zeros(X.shape).to(self.devices)
  // # for i in range(edge_index.shape[0]):
  // #    s = edge_index[i, 0]
  // #    d = edge_index[i, 1]
  // #    mi[d] += src_aggregate[i]
  // #    mo[s] += dst_aggregate[i]        
  // mi = mi.index_add(0, edge_index[:, 1], src_aggregate)
  // mo = mo.index_add(0, edge_index[:, 0], dst_aggregate)
  // M = torch.cat([mi, mo, X], dim=1)

CREATE_M_3NODE_LOOP:
  for(int node = 0; node < NHITS; node++){
    #pragma HLS unroll factor=1
    // #pragma HLS unroll factor=10
    // #pragma HLS unroll
    // #pragma HLS PIPELINE
CREATE_M_4PAR_LOOP:
    for(int par = 0; par < NPARHID; par++){
      // #pragma HLS unroll factor=1
      #pragma HLS unroll
      M[node*NPARHID3 + par + NPARHID2] = H[node*NPARHID + par];
    }
  }


}


void node_runner(data_t M[NHITS * NPARHID3], data_t H[NHITS * NPARHID]){
  // #pragma HLS PIPELINE
  node_net::input_t in1[N_INPUT_1_1];
  node_net::result_t out1[N_LAYER_8];

NODE_R_HIT_LOOP:
  for(int i = 0; i < NHITS; i++){
    #pragma HLS unroll factor=2
    for(int j = 0; j < N_INPUT_1_1; j++){
      #pragma HLS unroll
      in1[j] = M[i*N_INPUT_1_1 + j];
    }

    node_network_s(in1, out1);

    for(int j = 0; j < N_LAYER_8; j++){
      #pragma HLS unroll
      H[i*NPARHID + j] = out1[j];
    }
    for(int j = 0; j < NPARAMS; j++){
      #pragma HLS unroll
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
