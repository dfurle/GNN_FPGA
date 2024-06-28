#include "projectDefines.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

int main(int argc, char* argv[]){

  hls::stream<data_t> X_stream;
  hls::stream<i_data_t> ei_stream;
  hls::stream<data_t> e_stream;

  // data_t X[NHITS * NPARAMS];
  // i_data_t edge_i[NEDGES * 2];
  data_t e[NEDGES];
  data_t e_true[NEDGES];

  std::ifstream file("tb_inputs.dat");
  if(file.is_open()){
    std::string sa;
    for(int i = 0; i < NHITS * NPARAMS; i++){ // X
      getline(file, sa);
      X_stream.write(std::stof(sa));
    }
    for(int i = 0; i < NEDGES * 2; i++){ // ei
      getline(file, sa);
      // edge_i[i] = std::stoi(sa);
      ei_stream.write(std::stof(sa));
    }
    for(int i = 0; i < NEDGES; i++){ // e
      getline(file, sa);
      e_true[i] = std::stof(sa);
    }
  } else {
    printf("!!! Can't open input file!!\n");
    return 0;
  }


  printf("Running Network\n");
  runGraphNetwork(X_stream, ei_stream, e_stream);
  printf("\n\n");


  for (int i = 0; i < NEDGES; i++) {
    e[i] = e_stream.read();
  }

  int counter = 1;
  printf("True Outs:\n");
  for (int i = 0; i < NEDGES; i++) {
    if(float(e[i]) == 0)
      printf("_      ");
    else
    printf("1      ");
    if(counter++ % 10 == 0)
      printf("\n");
  }
  printf("\n\n");

  counter = 1;
  printf("Pred Outs:\n");
  for (int i = 0; i < NEDGES; i++) {
    if(float(e[i]) == 0)
      printf("_      ");
    else
      printf("%5.3f  ", float(e[i]));
    if(counter++ % 10 == 0)
      printf("\n");
  }
  printf("\n\n");


  counter = 1;
  printf("Diff True-Pred:\n");
  for (int i = 0; i < NEDGES; i++) {
    if(float(e[i]) == 0)
      printf(" _     ");
    else
    printf("%5.2f  ", float(e_true[i] - e[i]));
    if(counter++ % 10 == 0)
      printf("\n");
  }
  printf("\n\n");


}