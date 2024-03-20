#include "myproject_i.h"
#include "myproject_e.h"
#include "myproject_n.h"

#include "projectDefines.h"

#include <iostream>

/**
 * 
 * TODO: figure out these messages and how to control them
 *  - Multiple burst reads of length 25 and bit width 128 has been inferred on bundle 'aximm1'. These burst requests might be further partitioned into multiple requests during RTL generation, based on max_read_burst_length or max_write_burst_length settings.
 *  - The II Violation in module 'edge_network_1_Pipeline_EDGE_EDGE_LOOP' (loop 'EDGE_EDGE_LOOP'): Unable to schedule bus request operation ('aximm1_load_395_req', /home/denis/UBUNTU_TEST/graph_network/network/edge/myproject_e.cpp:75) on port 'aximm1' (/home/denis/UBUNTU_TEST/graph_network/network/edge/myproject_e.cpp:75) due to limited memory ports (II = 256). Please consider using a memory core with more ports or partitioning the array.
 *  - Automatically inferring inferring RAM_1WNR storage type for array 'Ri'. Use bind_storage pragma to override this type.
 * 
 * 
 * Figure out what this is using vitis analyzer---
 *     + runGraphNetwork_Pipeline_VITIS_LOOP_28_1                              |       -|   2.87|        -|          -|         -|        -|     -|        no|         -|           -|     67 (~0%)|    171 (~0%)|    -|
    |   o VITIS_LOOP_28_1                                                      |       -|   4.40|        -|          -|         1|        1|     -|       yes|         -|           -|            -|            -|    -|
    |  + runGraphNetwork_Pipeline_VITIS_LOOP_28_11                             |       -|   2.87|        -|          -|         -|        -|     -|        no|         -|           -|     67 (~0%)|    171 (~0%)|    -|
    |   o VITIS_LOOP_28_1                                                      |       -|   4.40|        -|          -|         1|        1|     -|       yes|         -|           -|            -|            -|    -|
    |  + runGraphNetwork_Pipeline_VITIS_LOOP_28_12                             |       -|   2.87|        -|          -|         -|        -|     -|        no|         -|           -|     67 (~0%)|    171 (~0%)|    -|
    |   o VITIS_LOOP_28_1                                                      |       -|   4.40|        -|          -|         1|        1|     -|       yes|         -|           -|            -|            -|    -|
    |  + runGraphNetwork_Pipeline_VITIS_LOOP_28_13                             |       -|   2.87|        -|          -|         -|        -|     -|        no|         -|           -|     67 (~0%)|    171 (~0%)|    -|
    |   o VITIS_LOOP_28_1   
 * 
 * 
 */

// #define RUN_PIPELINE


extern "C"{


// void runIteration(par_t H_0[NHITS], ei_t ei_0, int valid_edges){
//   par_t H_1[NHITS];
//   data_t e_1[NEDGES];
//   ei_t ei_1 = ei_0;
//   edge_network(H_0, ei_1, e_1, valid_edges);
//   node_network(H_0, ei_1, e_1, H_1);
// }


void runGraphNetwork(data_t X_arr[NHITS * NPARAMS], i_data_t edge_i_arr[NEDGES * 2], data_t e_arr[NEDGES], int valid_edges){
  #pragma HLS PIPELINE
  // #pragma HLS INLINE off

  ei_t ei_arr;
  for(int i = 0; i < NEDGES; i++){
    #pragma HLS UNROLL
    ei_arr[i] = edge_i_arr[i];
  }

  #ifndef RUN_PIPELINE
  par_t H_0[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_0 complete
  par_t H_1[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_1 complete

input:
  input_network(X_arr, H_0);
  
pass1:
  edge_network(H_0, ei_arr, e_arr, valid_edges);
  node_network(H_0, ei_arr, e_arr, H_1);

pass2:
  edge_network(H_1, ei_arr, e_arr, valid_edges);
  node_network(H_1, ei_arr, e_arr, H_0);

pass3:
  edge_network(H_0, ei_arr, e_arr, valid_edges);
  node_network(H_0, ei_arr, e_arr, H_1);

final:
  edge_network(H_1, ei_arr, e_arr, valid_edges);

  #else
  // INPUT
  par_t H_0[NHITS];
  ei_t ei_0 = ei_arr;
  input_network(X_arr, H_0);

  // ITER #1
  par_t H_1[NHITS];
  data_t e_1[NEDGES];
  ei_t ei_1 = ei_0;
  edge_network(H_0, ei_1, e_1, valid_edges);
  node_network(H_0, ei_1, e_1, H_1);
  

  // // ITER #2
  // par_t H_2[NHITS];
  // data_t e_2[NEDGES];
  // ei_t ei_2 = ei_1;
  // edge_network(H_1, ei_2, e_2, valid_edges);
  // node_network(H_1, ei_2, e_2, H_2);

  // // ITER #3
  // par_t H_3[NHITS];
  // data_t e_3[NEDGES];
  // ei_t ei_3 = ei_2;
  // edge_network(H_2, ei_3, e_3, valid_edges);
  // node_network(H_2, ei_3, e_3, H_3);

  // ENDING
  edge_network(H_1, ei_1, e_arr, valid_edges);
  // edge_network(H_3, ei_3, e_arr, valid_edges);
  #endif
}


void runner(data_t X_arr[NHITS * NPARAMS], i_data_t edge_index_arr[NEDGES * 2], data_t e_arr[NEDGES]){
  // #pragma HLS DATAFLOW
  #pragma HLS INTERFACE m_axi port=X_arr  offset=slave bundle=aximm1
  #pragma HLS INTERFACE m_axi port=edge_index_arr offset=slave bundle=aximm1
  #pragma HLS INTERFACE m_axi port=e_arr  offset=slave bundle=aximm1
  // #pragma HLS INTERFACE s_axilite port=X_arr  bundle=control
  // #pragma HLS INTERFACE s_axilite port=Ro_arr bundle=control
  // #pragma HLS INTERFACE s_axilite port=Ri_arr bundle=control
  // #pragma HLS INTERFACE s_axilite port=e_arr  bundle=control
  // #pragma HLS INTERFACE s_axilite port=return bundle=control

  // #pragma HLS ARRAY_RESHAPE variable = X_arr complete dim = 0
  // #pragma HLS ARRAY_RESHAPE variable = Ro_arr complete dim = 0
  // #pragma HLS ARRAY_RESHAPE variable = Ri_arr complete dim = 0
  // #pragma HLS ARRAY_RESHAPE variable = e_arr complete dim = 0

  // par_t read_H_arr[NHITS];
  // // #pragma HLS ARRAY_PARTITION variable=read_H_arr cyclic factor=NPARHID
  // par_t read_H2_arr[NHITS];
  // // #pragma HLS ARRAY_PARTITION variable=read_H2_arr cyclic factor=NPARHID


  data_t read_X_arr[NHITS * NPARAMS];
  i_data_t read_edge_index_arr[NEDGES * 2];
  data_t read_e_arr[NEDGES];
  #pragma HLS ARRAY_PARTITION variable=read_X_arr block factor=NHITS
  #pragma HLS ARRAY_PARTITION variable=read_edge_index_arr block factor=NEDGES
  #pragma HLS ARRAY_PARTITION variable=read_e_arr complete

READ_IN1_HITS:
  for(int i = 0; i < NHITS; i++){
    #pragma HLS unroll factor=1
    for(int j = 0; j < NPARAMS; j++){
      #pragma HLS unroll factor=1
      read_X_arr[i*NPARAMS + j] = X_arr[i*NPARAMS + j];
    }
  }

  int valid_edges = 0;

READ_IN_EDGE:
  for(int i = 0; i < NEDGES; i++){
    #pragma HLS unroll factor=1
    read_edge_index_arr[i*2] = edge_index_arr[i*2];
    read_edge_index_arr[i*2+1] = edge_index_arr[i*2+1];
    if(edge_index_arr[i*2] == i_data_t(-1) && valid_edges == 0)
      valid_edges = i;
  }

  runGraphNetwork(read_X_arr, read_edge_index_arr, read_e_arr, valid_edges);


  // par_t H_0[NHITS];
  // #pragma HLS ARRAY_PARTITION variable=H_0 complete
  // par_t H_1[NHITS];
  // #pragma HLS ARRAY_PARTITION variable=H_1 complete

  // input_network(X_arr, H_0);

  // edge_network(H_0, read_edge_index_arr, read_e_arr, valid_edges);

  // node_network(H_0, read_edge_index_arr, read_e_arr, H_1);

  // edge_network(H_1, read_edge_index_arr, read_e_arr, valid_edges);
  // node_network(H_1, read_edge_index_arr, read_e_arr, H_0);

  // edge_network(H_0, read_edge_index_arr, read_e_arr, valid_edges);
  // node_network(H_0, read_edge_index_arr, read_e_arr, H_1);

  // edge_network(H_1, read_edge_index_arr, read_e_arr, valid_edges);


WRITE_OUT:
  for(int i = 0; i < NEDGES; i++){
    if(read_edge_index_arr[i*2] != i_data_t(-1)){
      e_arr[i] = read_e_arr[i];
    }else{
      e_arr[i] = data_t(0.0);
    }
  }
}

}