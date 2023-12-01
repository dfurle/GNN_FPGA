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

  #ifdef STREAM
  H_t H;
  X_t X;
  R_t Ro, Ri;
  e_t e;

  data_t* ptr = (data_t*) X.begin();
  ptr = X_arr;

  hls::stream<H_t> stream_H2;
  hls::stream<H_t> stream_H;
  hls::stream<X_t> stream_X;
  hls::stream<R_t> stream_Ro;
  hls::stream<R_t> stream_Ri;
  hls::stream<e_t> stream_e;



  stream_X << X;
  input_network(stream_X, stream_H);

  std::cout << "H = np.array([ " << std::endl;
  for(int i = 0; i < NHITS; i++){
    std::cout << "[";
    for(int j = 0; j < NPARHID-1; j++){
      std::cout << float(H[i][j]) << ", ";
    }
    std::cout << float(H[i][NPARHID-1]);
    std::cout << "]," << std::endl;
  }
  std::cout << "])" << std::endl;


  // // ITER #1
  // stream_H >> H;

  // stream_H << H;
  // stream_Ro << Ro;
  // stream_Ri << Ri;
  // edge_network(stream_H, stream_Ro, stream_Ri, stream_e);

  // stream_H << H;
  // stream_Ro << Ro;
  // stream_Ri << Ri;
  // node_network(stream_H, stream_Ro, stream_Ri, stream_e, stream_H2);

  // // ITER #2
  // stream_H2 >> H;

  // stream_H << H;
  // stream_Ro << Ro;
  // stream_Ri << Ri;
  // edge_network(stream_H, stream_Ro, stream_Ri, stream_e);

  // stream_H << H;
  // stream_Ro << Ro;
  // stream_Ri << Ri;
  // node_network(stream_H, stream_Ro, stream_Ri, stream_e, stream_H2);

  // // Ending

  // stream_H2 >> H;

  // stream_H << H;
  // stream_Ro << Ro;
  // stream_Ri << Ri;
  // edge_network(stream_H, stream_Ro, stream_Ri, stream_e);

  // stream_e >> e;

  for(int i = 0; i < NEDGES; i++){
    // e_arr[i] = e[i];
    e_arr[i] = 0;
  }
  #endif

  #ifdef VECTOR
  H_t H, H2;
  X_t X;
  R_t Ro, Ri;
  e_t e;


  for(int i = 0; i < NHITS; i++){
    for(int j = 0; j < NPARAMS; j++){
      X[i][j] = X_arr[i*NPARAMS + j];
    }
    for(int j = 0; j < NEDGES; j++){
      Ro[i][j] = Ro_arr[i*NEDGES + j];
      Ri[i][j] = Ri_arr[i*NEDGES + j];
    }
  }

  std::cout << "X = np.array([" << std::endl;
  for(int i = 0; i < NHITS; i++){
    std::cout << "[";
    for(int j = 0; j < NPARAMS-1; j++){
      std::cout << float(X[i][j]) << ", ";
    }
    std::cout << float(X[i][NPARAMS-1]);
    std::cout << "]," << std::endl;
  }
  std::cout << "]" << std::endl;

  input_network(X, H);

  std::cout << "H = np.array([ " << std::endl;
  for(int i = 0; i < NHITS; i++){
    std::cout << "[";
    for(int j = 0; j < NPARHID-1; j++){
      std::cout << float(H[i][j]) << ", ";
    }
    std::cout << float(H[i][NPARHID-1]);
    std::cout << "]," << std::endl;
  }
  std::cout << "])" << std::endl;

  // // ITER #1
  // edge_network(H, Ro, Ri, e);
  // node_network(H, Ro, Ri, e, H2);
  // // ITER #2
  // edge_network(H2, Ro, Ri, e);
  // node_network(H2, Ro, Ri, e, H);
  // // Ending
  // edge_network(H, Ro, Ri, e);

  for(int i = 0; i < NEDGES; i++){
    // e_arr[i] = e[i];
    e_arr[i] = 0;
  }
  #endif

  #ifdef ARRAY
  data_t read_H_arr[NHITS * NPARHID];
  // #pragma HLS ARRAY_PARTITION variable=read_H_arr complete
  data_t read_H2_arr[NHITS * NPARHID];
  // #pragma HLS ARRAY_PARTITION variable=read_H2_arr complete


  data_t read_X_arr[NHITS * NPARAMS];
  i_data_t read_edge_index_arr[NEDGES * 2];
  data_t read_e_arr[NEDGES];
  // #pragma HLS ARRAY_PARTITION variable=read_X_arr complete
  // #pragma HLS ARRAY_PARTITION variable=read_edge_index_arr complete
  // #pragma HLS ARRAY_PARTITION variable=read_edge_index_arr block factor=2
  // #pragma HLS ARRAY_PARTITION variable=read_e_arr complete

READ_IN1_HITS:
  for(int i = 0; i < NHITS; i++){
    #pragma HLS unroll factor=1
READ_IN_INNER_HITS:
    for(int j = 0; j < NPARAMS; j++){
      #pragma HLS unroll factor=1
      read_X_arr[i*NPARAMS + j] = X_arr[i*NPARAMS + j];
    }
  }

  int valid_edges = 0;

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
  #endif
  
}

}