#include "projectDefines.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>



void runFPGA(std::string filename, bool doprint, std::vector<float>& _e_pred, std::vector<float>& _e_true, std::vector<float>& _e_pyth){


  hls::stream<data_t> X_stream;
  hls::stream<i_data_t> ei_stream;
  hls::stream<data_t> e_stream;

  // data_t X[NHITS * NPARAMS];
  // i_data_t edge_i[NEDGES * 2];
  data_t e[NEDGES];
  data_t e_true[NEDGES];
  data_t e_pyth[NEDGES];



  // std::ifstream file("../tb_files/tb_inputs.dat");
  std::ifstream file(filename);
  if(file.is_open()){
    std::string sa;
    for(int i = 0; i < NHITS * NPARAMS; i++){ // X
      getline(file, sa);
      if(doprint)
        printf("> n:%i [%s]\n", i/NPARAMS, sa.c_str());
      X_stream.write(std::stof(sa));
    }
    for(int i = 0; i < NEDGES * 2; i++){ // ei
      getline(file, sa);
      // edge_i[i] = std::stoi(sa);
      if(doprint)
        printf("> i:%i [%s]\n", i/2, sa.c_str());
      ei_stream.write(std::stof(sa));
    }
    for(int i = 0; i < NEDGES; i++){ // e
      getline(file, sa);
      if(doprint)
        printf("> i:%i [%s]\n", i, sa.c_str());
      e_true[i] = std::stof(sa);
      _e_true.push_back(float(e_true[i]));
    }
    for(int i = 0; i < NEDGES; i++){ // e pytorch preds
      getline(file, sa);
      if(doprint)
        printf("> i:%i [%s]\n", i, sa.c_str());
      e_pyth[i] = std::stof(sa);
      _e_pyth.push_back(float(e_pyth[i]));
    }
  } else {
    printf("!!! Can't open input file!!\n");
    return;
  }


  printf("Running Network %s\n", filename.c_str());
  runGraphNetwork(X_stream, ei_stream, e_stream);
  // printf("\n");


  for (int i = 0; i < NEDGES; i++) {
    e[i] = e_stream.read();
    _e_pred.push_back(float(e[i]));
  }


  if(doprint){
  // if(true){
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
      if(abs(float(e_pyth[i]) - float(e[i])) <= 0.01) // error 0.01 to just zero off
        printf(" _     ");
      else
        printf("%5.2f  ", float(e_pyth[i]) - float(e[i]));
      if(counter++ % 10 == 0)
        printf("\n");
    }
    printf("\n\n");

    counter = 1;
    printf("Diff True-Pred:\n");
    for (int i = 0; i < NEDGES; i++) {
      if(abs(float(e_true[i]) - float(e[i])) <= 0.01)
        printf(" _     ");
      else
        printf("%5.2f  ", float(e_true[i]) - float(e[i]));
      if(counter++ % 10 == 0)
        printf("\n");
    }
    printf("\n\n");
  }

  std::ofstream file_write("../../../../../../py_event_preds.txt");
  if(file_write.is_open()){
    for(int i = 0; i < NEDGES; i++){
      file_write << std::to_string(float(e[i])) << " ";
    }
    file_write << "\n";
    file_write.close();
  } else {
    printf("Failed to open preds file for writing!!\n");
  }
  

}


int main(int argc, char* argv[]){
  printf("RUNNING CSIM\n");

  

  std::vector<float> e_pred;
  std::vector<float> e_true;
  std::vector<float> e_pyth;


  int numTestGraphs = 10;
  for(int i = 0; i < numTestGraphs; i++){
    runFPGA("fpga_data/tb_inputs_" + std::to_string(i) + ".dat", true, e_pred, e_true, e_pyth);
  }

  // printf("Opening file: \n\n\n\n");
  // runFPGA("fpga_data/tb_inputs_0.dat", false, e_pred, e_true, e_pyth);


  int length = e_pred.size();
  printf("Length: %d\n", length);

  const int numberCuts = 101;
  float cutThresh[numberCuts];
  for(int i = 0; i < numberCuts; i++){
    cutThresh[i] = i/float(numberCuts-1);
  }


  float accuracy[numberCuts];
  float truePositive[numberCuts];
  float falsePositive[numberCuts];
  float purity[numberCuts];
  float efficiency[numberCuts];

  for(int ct = 0; ct < numberCuts; ct++){
    //                            truth     -> predicted
    int numTruePositive = 0;   // true edge -> true edge
    int numFalsePositive = 0;  // fake edge -> true edge
    int numTrueNegative = 0;   // fake edge -> fake edge
    int numFalseNegative = 0;  // true edge -> fake edge
    for(int i = 0; i < length; i++){
      // if(e_true[i] >= 0.5 && e_pred[i] >= cutThresh[ct])
      //   numTruePositive++;
      // if(e_true[i] >= 0.5 && e_pred[i] <  cutThresh[ct])
      //   numFalseNegative++;
      // if(e_true[i] < 0.5  && e_pred[i] >= cutThresh[ct])
      //   numFalsePositive++;
      // if(e_true[i] < 0.5  && e_pred[i] <  cutThresh[ct])
      //   numTrueNegative++;


      if(e_true[i] >= 0.5 && e_pyth[i] >= cutThresh[ct])
        numTruePositive++;
      if(e_true[i] >= 0.5 && e_pyth[i] <  cutThresh[ct])
        numFalseNegative++;
      if(e_true[i] < 0.5  && e_pyth[i] >= cutThresh[ct])
        numFalsePositive++;
      if(e_true[i] < 0.5  && e_pyth[i] <  cutThresh[ct])
        numTrueNegative++;
    }
    accuracy[ct] = (numTruePositive + numTrueNegative) / float(length);
    truePositive[ct] = float(numTruePositive) / (numTruePositive + numFalseNegative);
    falsePositive[ct] = float(numFalsePositive) / (numTrueNegative + numFalsePositive);
    purity[ct] = float(numTruePositive) / (numTruePositive + numFalsePositive);
    efficiency[ct] = float(numTruePositive) / (numTruePositive + numFalseNegative);
    // printf("Cut %.3f\n", cutThresh[ct]);
    printf("%5d %5d %5d %5d\n", numTruePositive, numFalsePositive, numTrueNegative, numFalseNegative);
  }
  // printf("\n");



  // std::ofstream file;
  // file.open("variables.txt");

  printf("cuts = [");
  for(int i = 0; i < numberCuts; i++){
    printf("%.3f, ", cutThresh[i]);
  }
  printf("]\n");

  printf("accs = [");
  for(int i = 0; i < numberCuts; i++){
    printf("%.3f, ", accuracy[i]);
  }
  printf("]\n");

  printf("truePos = [");
  for(int i = 0; i < numberCuts; i++){
    printf("%.3f, ", truePositive[i]);
  }
  printf("]\n");

  printf("falsePos = [");
  for(int i = 0; i < numberCuts; i++){
    printf("%.3f, ", falsePositive[i]);
  }
  printf("]\n");

  printf("purity = [");
  for(int i = 0; i < numberCuts; i++){
    printf("%.3f, ", purity[i]);
  }
  printf("]\n");

  printf("eff = [");
  for(int i = 0; i < numberCuts; i++){
    printf("%.3f, ", efficiency[i]);
  }
  printf("]\n");


  std::ofstream file("../../../../../../py_diff_vals.txt");
  if(file.is_open()){
    for(int i = 0; i < length; i++){
      file << std::to_string(e_pyth[i] - e_pred[i]) << " ";
    }
    file << "\n";
    file.close();
  } else {
    printf("Failed to open file for writing!!\n");
  }





}