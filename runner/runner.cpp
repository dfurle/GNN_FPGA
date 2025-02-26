#include "myproject_i.h"
#include "myproject_e.h"
#include "myproject_n.h"

#include "projectDefines.h"

#include <iostream>

// #define RUN_PIPELINE



// #define THETA_MAX_LONG data_t(0.700)   // 35deg
// #define THETA_MAX_SHORT data_t(1.19175)   // 50 deg
// #define MAX_DIST data_t(80)
// #define DIST_LAYER data_t(5.5)
// #define SIZE_LAYER data_t(15)
// #define DIST_MODULE_1_2_3 data_t(55)
// #define DIST_MODULE_3_4 data_t(33.5)
// #define DIST_INTERLAYER data_t(7.33)
// #define Z_275_SPLIT data_t(275)

#define THETA_MAX_LONG 0.49   // tan(35deg)^2 = (0.7)^2
#define THETA_MAX_SHORT 1.420268   // tan(50 deg)^2 = (1.19175)^2
#define MAX_DIST 6400  // 80^2
#define DIST_LAYER 5.5
#define SIZE_LAYER 15.0
#define DIST_MODULE_1_2_3 54.0
#define DIST_MODULE_3_4 33.5
#define DIST_INTERLAYER 7.33
#define Z_275_SPLIT 99.0 // 275 - 176

int to_graph(data_t X[NHITS * NPARAMS], i_data_t ei[NEDGES*2]){

  int counter = 0;
  for(int i = 0; i < NEDGES*2; i++){
    #pragma HLS UNROLL
    ei[i] = i_data_t(-1);
  }
  

  for(int i = 0; i < NHITS; i++){
    data_t hit1_x = X[NPARAMS*i+0];
    data_t hit1_y = X[NPARAMS*i+1];
    data_t hit1_z = X[NPARAMS*i+2];
    for(int j = 0; j < NHITS; j++){
      // #pragma HLS PIPELINE
      #pragma HLS UNROLL factor=10
      if(counter >= NEDGES)
        break;
      data_t hit2_x = X[NPARAMS*j+0];
      data_t hit2_y = X[NPARAMS*j+1];
      data_t hit2_z = X[NPARAMS*j+2];

      // float hit1_x = X[NPARAMS*i+0];
      // float hit1_y = X[NPARAMS*i+1];
      // float hit1_z = X[NPARAMS*i+2];
      // float hit2_x = X[NPARAMS*j+0];
      // float hit2_y = X[NPARAMS*j+1];
      // float hit2_z = X[NPARAMS*j+2];
      if(hit1_z == 0)
        continue;
      if(hit2_z == 0)
        continue;

      data_t diff_x = hit2_x - hit1_x;
      data_t diff_y = hit2_y - hit1_y;
      data_t diff_z = hit2_z - hit1_z;
      data_t xy_mag2 = diff_x*diff_x + diff_y*diff_y;
      // data_t dist = xy_mag + diff_z*diff_z;

      // float diff_x = hit2_x - hit1_x;
      // float diff_y = hit2_y - hit1_y;
      // float diff_z = hit2_z - hit1_z;
      // float xy_mag = sqrt(diff_x*diff_x + diff_y*diff_y);
      // float dist = xy_mag*xy_mag + diff_z*diff_z;

      // printf("Edge %4d | {%3d, %3d}\n", counter, i, j);
      // printf("  Hit1 %3d\n", i);
      // printf("    %5.3f, %5.3f, %5.3f\n", float(hit1_x), float(hit1_y), float(hit1_z));
      // printf("  Hit2 %3d\n", j);
      // printf("    %5.3f, %5.3f, %5.3f\n", float(hit2_x), float(hit2_y), float(hit2_z));
      // printf("  Diff:\n");
      // printf("    %5.3f, %5.3f, %5.3f\n", float(diff_x), float(diff_y), float(diff_z));
      // printf("  Mag XY: %5.3f\n", float(xy_mag));
      // printf("  Mag XYZ: %5.3f\n", float(dist));
      if(diff_z <= 0)
        continue;
      // data_t theta = abs(float(xy_mag / diff_z));
      data_t theta = xy_mag2 / (diff_z*diff_z);
      if(theta < 0)
        theta = -theta;
      // printf("  Theta: %5.3f\n", float(theta));

      if(diff_z >= DIST_LAYER && theta > THETA_MAX_LONG)
        continue;
      if(diff_z < DIST_LAYER && theta > THETA_MAX_SHORT)
        continue;
      // if(dist > MAX_DIST || dist < 0)
      //   continue;
      if(diff_z < SIZE_LAYER and diff_z > DIST_INTERLAYER)
        continue;
      if(diff_z > SIZE_LAYER){
        if(hit1_z < Z_275_SPLIT && diff_z > DIST_MODULE_1_2_3)
          continue;
        if(hit1_z >= Z_275_SPLIT && diff_z > DIST_MODULE_3_4)
          continue;
      }
      
      // printf("  !! Passed !!\n");
      ei[2*counter] = i;
      ei[2*counter+1] = j;
      counter++;
      // printf("\n");
    }
    if(counter >= NEDGES)
      break;
  }
  return counter;
}


void network_pass(int valid_edges, par_t H_in[NHITS], par_t H_out[NHITS], i_data_t ei_in[NEDGES*2], i_data_t ei_out[NEDGES*2]){

  // // is a global copy, but needs to be flowed
  // i_data_t ei[NEDGES*2];
  // #pragma HLS ARRAY_PARTITION variable=ei complete

  // not needed between different passes
  par_t inbound[NEDGES];
  par_t outbound[NEDGES];
  #pragma HLS ARRAY_PARTITION variable=inbound complete
  #pragma HLS ARRAY_PARTITION variable=outbound complete
  // only used in final output

  edge_network(H_in, ei_in, inbound, outbound);
  node_network(H_in, H_out, inbound, outbound);

  for(int i = 0; i < 2*NEDGES; i++){
    #pragma HLS UNROLL
    ei_out[i] = ei_in[i];
  }

}



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

  printf("\n\n\n\n\n\n");
  printf("Starting to_graph()\n");

  i_data_t ei[NEDGES*2];
  #pragma HLS ARRAY_PARTITION variable=ei complete
  int valid_edges = to_graph(X, ei);

  printf("Finished to_graph()\n");

  int valid_edges_true = 0;
  i_data_t ei_true[NEDGES*2];
  #pragma HLS ARRAY_PARTITION variable=ei complete
  for(int i = 0; i < NEDGES * 2; i++){
    #pragma HLS PIPELINE
    ei_true[i] = in_ei_stream.read();
    if(ei_true[i] == i_data_t(-1) && valid_edges == 0){
      valid_edges = i/2;
      // keep reading stream to finish
    }
  }
  if(valid_edges == 0){
    valid_edges = NEDGES;
  }

  printf("EI True | EI Built\n");
  int num_correct = 0;
  for(int i = 0; i < NEDGES; i++){
    printf(" [%5d %5d]  |  [%5d %5d]", int(ei_true[2*i]), int(ei_true[2*i+1]), int(ei[2*i]), int(ei[2*i+1]));
    if(ei_true[2*i] == ei[2*i] && ei_true[2*i+1] == ei[2*i+1])
      num_correct += 1;
    else
      printf(" !");
    printf("\n");
  }
  if(num_correct == NEDGES){
    printf("\n\n ALL CORRECT \n\n\n");
  } else {
    printf("\n\n SOME FAILED \n\n\n");
  }

  data_t e[NEDGES];
  #pragma HLS ARRAY_PARTITION variable=e complete

  #ifdef RUN_PIPELINE
  par_t H_0[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_0 complete
  par_t H_1[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_1 complete
  par_t H_2[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_2 complete
  par_t H_3[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_3 complete

input:
  input_network(X, H_0);
pass1:
  network_pass(valid_edges, H_0, H_1, ei, ei);
pass2:
  network_pass(valid_edges, H_1, H_2, ei, ei);
pass3:
  network_pass(valid_edges, H_2, H_3, ei, ei);
final:
  edge_predictor(valid_edges, H_3, ei, e);

  for(int i = 0; i < NEDGES; i++){
    out_e_stream.write(e[i]);
  }

  #else

  par_t H_0[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_0 complete
  par_t H_1[NHITS];
  #pragma HLS ARRAY_PARTITION variable=H_1 complete

  // par_t inbound[NEDGES];
  // par_t outbound[NEDGES];
  // #pragma HLS ARRAY_PARTITION variable=inbound complete
  // #pragma HLS ARRAY_PARTITION variable=outbound complete

input:
  input_network(X, H_0);
pass1:
  network_pass(valid_edges, H_0, H_1, ei, ei);
  // edge_network(H_0, ei, inbound, outbound);
  // node_network(H_0, H_1, inbound, outbound);
pass2:
  network_pass(valid_edges, H_1, H_0, ei, ei);
  // edge_network(H_1, ei, inbound, outbound);
  // node_network(H_1, H_0, inbound, outbound);
pass3:
  network_pass(valid_edges, H_0, H_1, ei, ei);
  // edge_network(H_0, ei, inbound, outbound);
  // node_network(H_0, H_1, inbound, outbound);
final:
  edge_predictor(valid_edges, H_1, ei, e);

  for(int i = 0; i < NEDGES; i++){
    out_e_stream.write(e[i]);
  }

  // 812s   <- with the inbound unrolled?
  // 921s   <- current

  #endif


}

}
