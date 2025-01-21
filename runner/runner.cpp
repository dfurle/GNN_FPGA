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


void runGraphNetwork(hls::stream<data_t>& in_X_stream, hls::stream<i_data_t>& in_ei_stream, hls::stream<data_t>& out_e_stream){
  // #pragma HLS DATAFLOW
  // #pragma HLS INLINE off

  data_t X[NHITS * NPARAMS];
  #pragma HLS ARRAY_PARTITION variable=X complete
  for(int i = 0; i < NHITS * NPARAMS; i++){
    #pragma HLS PIPELINE
    X[i] = in_X_stream.read();
  }

    // |+ runGraphNetwork                                                        |  Timing|  -0.57|     3624|  1.812e+04|         -|     3625|     -|        no|  21 (~0%)|  1464 (21%)|  145707 (6%)|  116327 (9%)|    -|
    // | + createM                                                               |  Timing|  -0.04|     1335|  6.675e+03|         -|     1335|     -|        no|         -|    10 (~0%)|   24625 (1%)|   20605 (1%)|    -|
    // |  + createM_Pipeline_CREATE_M_ZERO_LOOP                                  |       -|   2.11|      152|    760.000|         -|      152|     -|        no|         -|           -|     10 (~0%)|     57 (~0%)|    -|
    // |   o CREATE_M_ZERO_LOOP                                                  |       -|   4.40|      150|    750.000|         1|        1|   150|       yes|         -|           -|            -|            -|    -|
    // |  + createM_Pipeline_CREATE_M_1EDGE_LOOP                                 |  Timing|  -0.04|     1026|  5.130e+03|         -|     1026|     -|        no|         -|    10 (~0%)|    427 (~0%)|   12425 (1%)|    -|
    // |   o CREATE_M_1EDGE_LOOP                                                 |      II|   4.40|     1024|  5.120e+03|         4|        4|   256|       yes|         -|           -|            -|            -|    -|
    // |  + createM_Pipeline_CREATE_M_3NODE_LOOP                                 |       -|   2.00|      152|    760.000|         -|      152|     -|        no|         -|           -|    179 (~0%)|   7978 (~0%)|    -|
    // |   o CREATE_M_3NODE_LOOP                                                 |       -|   4.40|      150|    750.000|         2|        1|   150|       yes|         -|           -|            -|            -|    -|
    // | + node_runner         


  // printf("\n\n");
  // printf("X input:\n");
  // for(int i = 0; i < NHITS; i++){
  //   printf("%3d: ", i);
  //   for(int j = 0; j < NPARAMS; j++){
  //     // printf("%.3f, ", float(X[i][j]));
  //     std::cout << X[i*NPARAMS + j] << ", ";
  //   }
  //   printf("\n");
  // }
  // printf("\n\n");

  int valid_edges = 0;
  i_data_t ei[NEDGES*2];
  #pragma HLS ARRAY_PARTITION variable=ei complete
  for(int i = 0; i < NEDGES * 2; i++){
    #pragma HLS PIPELINE
    ei[i] = in_ei_stream.read();
    if(ei[i] == i_data_t(-1) && valid_edges == 0){
      valid_edges = i/2;
      // keep reading stream to finish
    }
  }
  if(valid_edges == 0){
    valid_edges = NEDGES;
  }

  // printf("\n\n");
  // printf("ei input:\n");
  // for(int i = 0; i < NEDGES; i++){
  //   for(int j = 0; j < 2; j++){
  //     printf("%d, ", int(ei[i*2+j]));
  //   }
  //   printf("\n");
  // }
  // printf("\n\n");

  data_t e[NEDGES];
  #pragma HLS ARRAY_PARTITION variable=e complete


  // printf("valid edges: %d\n", valid_edges);

  #ifndef RUN_PIPELINE
  par_t H_0[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_0 complete
  par_t H_1[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_1 complete

  par_t inbound[NEDGES];
  par_t outbound[NEDGES];
  // #pragma HLS ARRAY_PARTITION variable=B complete

  for(int i = 0; i < NEDGES; i++){
    inbound[i] = par_t(0);
    outbound[i] = par_t(0);
  }

input:
  input_network(X, H_0);

  // printf("\n\n");
  // printf("H_0 after input:\n");
  // for(int i = 0; i < NHITS; i++){
  //   for(int j = 0; j < NPARHID; j++){
  //     printf("%.3f, ", float(H_0[i][j]));
  //   }
  //   printf("\n");
  // }
  // printf("\n\n");
  
pass1:

  edge_network(H_0, ei, e, valid_edges, inbound, outbound);


  // printf("inbound[0]:\n");
  // for(int i = 0; i < NPARHID; i++){
  //   printf("%.3f, ", float(inbound[0][i]));
  // }
  // printf("\n");
  // printf("outbound[0]:\n");
  // for(int i = 0; i < NPARHID; i++){
  //   printf("%.3f, ", float(outbound[0][i]));
  // }
  // printf("\n\n");

  // printf("e first:\n");
  // for(int i = 0; i < NEDGES; i++){
  //   printf("%.3f, ", float(e[i]));
  //   if(i % 9 == 8)
  //     printf("\n");
  // }
  // printf("\n\n");

  node_network(H_0, ei, e, H_1, inbound, outbound);

  // printf("H_1 first:\n");
  // for(int i = 0; i < NHITS; i++){
  //   for(int j = 0; j < NPARHID; j++){
  //     printf("%.3f, ", float(H_1[i][j]));
  //   }
  //   printf("\n");
  // }
  // printf("\n\n");

pass2:
  edge_network(H_1, ei, e, valid_edges, inbound, outbound);

  // printf("e second:\n");
  // for(int i = 0; i < NEDGES; i++){
  //   printf("%.3f, ", float(e[i]));
  //   if(i % 9 == 8)
  //     printf("\n");
  // }
  // printf("\n\n");

  node_network(H_1, ei, e, H_0, inbound, outbound);


  // printf("H_0 second:\n");
  // for(int i = 0; i < NHITS; i++){
  //   for(int j = 0; j < NPARHID; j++){
  //     printf("%.3f, ", float(H_0[i][j]));
  //   }
  //   printf("\n");
  // }
  // printf("\n\n");


pass3:
  edge_network(H_0, ei, e, valid_edges, inbound, outbound);

  // printf("e third:\n");
  // for(int i = 0; i < NEDGES; i++){
  //   printf("%.3f, ", float(e[i]));
  //   if(i % 9 == 8)
  //     printf("\n");
  // }
  // printf("\n\n");

  node_network(H_0, ei, e, H_1, inbound, outbound);

final:
  edge_network(H_1, ei, e, valid_edges, inbound, outbound);


  for(int i = 0; i < NEDGES; i++){
    out_e_stream.write(e[i]);
  }

  #else

  par_t H_0[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_0 complete

  par_t H_1[NHITS];
  data_t e_1[NEDGES];
  #pragma HLS ARRAY_PARTITION variable=H_1 complete
  #pragma HLS ARRAY_PARTITION variable=e_1 complete

  par_t H_2[NHITS];
  data_t e_2[NEDGES];
  #pragma HLS ARRAY_PARTITION variable=H_2 complete
  #pragma HLS ARRAY_PARTITION variable=e_2 complete

  par_t H_3[NHITS];
  data_t e_3[NEDGES];
  #pragma HLS ARRAY_PARTITION variable=H_3 complete
  #pragma HLS ARRAY_PARTITION variable=e_3 complete

  // INPUT
  ei_t ei_0 = ei_arr;
  input_network(X_arr, H_0);

  // ITER #1
  ei_t ei_1 = ei_0;
  edge_network(H_0, ei_1, e_1, valid_edges);
  node_network(H_0, ei_1, e_1, H_1);
  
  // // ITER #2
  ei_t ei_2 = ei_1;
  edge_network(H_1, ei_2, e_2, valid_edges);
  node_network(H_1, ei_2, e_2, H_2);

  // // ITER #3
  ei_t ei_3 = ei_2;
  edge_network(H_2, ei_3, e_3, valid_edges);
  node_network(H_2, ei_3, e_3, H_3);

  // ENDING
  ei_t ei_4 = ei_3;
  edge_network(H_3, ei_4, e_arr, valid_edges);
  #endif
}

}