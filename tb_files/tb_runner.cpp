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
  data_t e_pyth[NEDGES];

  // std::ifstream file("../tb_files/tb_inputs.dat");
  std::ifstream file("tb_inputs.dat");
  if(file.is_open()){
    std::string sa;
    for(int i = 0; i < NHITS * NPARAMS; i++){ // X
      getline(file, sa);
      printf("> n:%i [%s]\n", i/NPARAMS, sa.c_str());
      X_stream.write(std::stof(sa));
    }
    for(int i = 0; i < NEDGES * 2; i++){ // ei
      getline(file, sa);
      // edge_i[i] = std::stoi(sa);
      printf("> i:%i [%s]\n", i/2, sa.c_str());
      ei_stream.write(std::stof(sa));
    }
    for(int i = 0; i < NEDGES; i++){ // e
      getline(file, sa);
      printf("> i:%i [%s]\n", i, sa.c_str());
      e_true[i] = std::stof(sa);
    }
    for(int i = 0; i < NEDGES; i++){ // e pytorch preds
      getline(file, sa);
      printf("> i:%i [%s]\n", i, sa.c_str());
      e_pyth[i] = std::stof(sa);
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
    if(float(e_true[i]) == 0)
      printf("_      ");
    else
      printf("1      ");
    if(counter++ % 10 == 0)
      printf("\n");
  }
  printf("\n\n");

  counter = 1;
  printf("Pyth Outs:\n");
  for (int i = 0; i < NEDGES; i++) {
    if(float(e_pyth[i]) == 0)
      printf("_      ");
    else
      printf("%5.3f  ", float(e_pyth[i]));
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
  printf("Diff Pyth-Pred:\n");
  for (int i = 0; i < NEDGES; i++) {
    if(float(e[i]) == 0)
      printf(" _     ");
    else
    printf("%5.2f  ", float(e_pyth[i] - e[i]));
    if(counter++ % 10 == 0)
      printf("\n");
  }
  printf("\n\n");


}