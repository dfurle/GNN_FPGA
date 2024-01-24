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
 */



extern "C"{


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

  par_t read_H_arr[NHITS];
  // #pragma HLS ARRAY_PARTITION variable=read_H_arr cyclic factor=NPARHID
  par_t read_H2_arr[NHITS];
  // #pragma HLS ARRAY_PARTITION variable=read_H2_arr cyclic factor=NPARHID


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

  input_network(read_X_arr, read_H_arr);
  // ITER #1
  edge_network(read_H_arr, read_edge_index_arr, read_e_arr, valid_edges);
  node_network(read_H_arr, read_edge_index_arr, read_e_arr, read_H2_arr);
  // ITER #2
  edge_network(read_H2_arr, read_edge_index_arr, read_e_arr, valid_edges);
  node_network(read_H2_arr, read_edge_index_arr, read_e_arr, read_H_arr);
  // ITER #3
  edge_network(read_H_arr, read_edge_index_arr, read_e_arr, valid_edges);
  node_network(read_H_arr, read_edge_index_arr, read_e_arr, read_H2_arr);
  // Ending
  edge_network(read_H2_arr, read_edge_index_arr, read_e_arr, valid_edges);


WRITE_OUT:
  for(int i = 0; i < NEDGES; i++){
    if(read_edge_index_arr[i*2] != i_data_t(-1))
      e_arr[i] = read_e_arr[i];
    else{
      e_arr[i] = data_t(0.0);
    }
  }
  
}

}