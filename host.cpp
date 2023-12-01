#include "helper.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "projectDefines.h"

// #include "ap_fixed.h"

// static const int DATA_SIZE = 1024; // 4096;

// #define N_INPUT_1_1 24
// #define N_LAYER_8 1

static const std::string error_message =
    "Error: Result mismatch:\n"
    "i = %d CPU result = %d Device result = %d\n";

int setupDevice(std::vector<cl::Device>& devices, cl::Device& device){
  bool found_device = false;
  // traversing all Platforms To find Xilinx Platform and targeted
  // Device in Xilinx Platform
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  std::cout << "Scanning Platforms" << std::endl;
  std::cout << "number: " << platforms.size() << std::endl;
  for (size_t i = 0; (i < platforms.size()) & (found_device == false); i++) {
    cl::Platform platform = platforms[i];
    std::string platformName = platform.getInfo<CL_PLATFORM_NAME>();
    std::cout << platformName << std::endl;
    if (platformName == "Xilinx") {
      devices.clear();
      platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices);
      if (devices.size()) {
        device = devices[0];
        found_device = true;
        break;
      }
    }
  }
  if (found_device == false) {
    std::cout << "Error: Unable to find Target Device "
              << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    return EXIT_FAILURE;
  }
  devices.resize(1);
  return 0;
}

void setupRunGraph(cl::Program& program, cl::Context& context, cl::CommandQueue& q, float *inX, int *in_ei, float *out1){

  cl::Kernel kernel(program, "runner");
  // cl::Kernel kernel(program, "input_network");
  // cl::Kernel inkernel(program, "input_network_1");
  // cl::Kernel e1kernel(program, "edge_network_1");
  // cl::Kernel n1kernel(program, "node_network_1");
  // cl::Kernel e2kernel(program, "edge_network_2");
  // cl::Kernel n2kernel(program, "node_network_2");
  // cl::Kernel eoutkernel(program, "edge_network_out");

  // Compute the size of array in bytes
  size_t in_X_size = sizeof(data_t) * NHITS * NPARAMS;
  size_t in_i_size = sizeof(i_data_t) * 2 * NEDGES;
  size_t out_e_size = sizeof(data_t) * NEDGES;

  std::cout << "in_X_size:  " << in_X_size << std::endl;
  std::cout << "in_i_size: " << in_i_size << std::endl;
  std::cout << "out_e_size: " << out_e_size << std::endl;

  // These commands will allocate memory on the Device. The cl::Buffer objects
  // can be used to reference the memory locations on the device.
  cl::Buffer buffer_X(context, CL_MEM_READ_ONLY, in_X_size);
  cl::Buffer buffer_i(context, CL_MEM_READ_ONLY, in_i_size);
  cl::Buffer buffer_e(context, CL_MEM_WRITE_ONLY, out_e_size);

  // set the kernel Arguments
  int narg = 0;
  kernel.setArg(narg++, buffer_X);
  kernel.setArg(narg++, buffer_i);
  // kernel.setArg(narg++, buffer_H);
  kernel.setArg(narg++, buffer_e);

  // We then need to map our OpenCL buffers to get the pointers
  data_t *ptr_X = (data_t *)q.enqueueMapBuffer(buffer_X, CL_TRUE, CL_MAP_WRITE, 0, in_X_size);
  i_data_t *ptr_i = (i_data_t *)q.enqueueMapBuffer(buffer_i, CL_TRUE, CL_MAP_WRITE, 0, in_i_size);
  data_t *ptr_e = (data_t *)q.enqueueMapBuffer(buffer_e, CL_TRUE, CL_MAP_READ, 0, out_e_size);

  std::cout << "setting input data" << std::endl;

  // setting input data
  std::cout << "X: " << std::endl;
  for (int i = 0; i < NHITS * NPARAMS; i++) {
    ptr_X[i] = inX[i];
    // ptr_X[i] = i * 1.59256392;
    std::cout << ptr_X[i] << " ";
  }
  std::cout << "\n\n\n";
  std::cout << std::endl;

  std::cout << "edge_index: " << std::endl;
  for (int i = 0; i < 2 * NEDGES; i++) {
    ptr_i[i] = i_data_t(in_ei[i]);
    // ptr_Ro[i] = i * 0.12542476;
    // ptr_Ri[i] = i * 0.78654325;
    // std::cout << "(" << ptr_Ro[i] << ", " << ptr_Ri[i] << ")  ";
    // if(i % NEDGES == NEDGES-1)
    //   std::cout << "\n\n\n";
  }
  std::cout << "\n\n\n";
  std::cout << std::endl;

  for (int i = 0; i < NEDGES; i++) {
    ptr_e[i] = 0.1256246;
  }

  // Data will be migrated to kernel space
  q.enqueueMigrateMemObjects({buffer_X}, 0); // 0 means from host
  q.enqueueMigrateMemObjects({buffer_i}, 0); // 0 means from host

  // Launch the Kernel
  q.enqueueTask(kernel);
  // q.enqueueTask(inkernel);
  // q.enqueueTask(e1kernel);
  // q.enqueueTask(n1kernel);
  // q.enqueueTask(e2kernel);
  // q.enqueueTask(n2kernel);
  // q.enqueueTask(eoutkernel);

  // The result of the previous kernel execution will need to be retrieved in
  // order to view the results. This call will transfer the data from FPGA to
  // source_results vector
  q.enqueueMigrateMemObjects({buffer_e}, CL_MIGRATE_MEM_OBJECT_HOST);

  q.finish();

  std::cout << "OUTPUT: " << std::endl;

  // setting input data
  for (int i = 0; i < NEDGES; i++) {
    out1[i] = ptr_e[i];
    std::cout << out1[i] << ", ";
  }
  std::cout << std::endl;

  q.enqueueUnmapMemObject(buffer_X, ptr_X);
  q.enqueueUnmapMemObject(buffer_i, ptr_i);
  q.enqueueUnmapMemObject(buffer_e, ptr_e);
  q.finish();
}

int main(int argc, char *argv[]) {

  // TARGET_DEVICE macro needs to be passed from gcc command line
  if (argc < 2 || argc > 3) {
    std::cout << "Usage: " << argv[0] << "<xclbin>" << std::endl;
    return EXIT_FAILURE;
  }
  // Creates a vector of DATA_SIZE elements with an initial value of 10 and 32
  // using customized allocator for getting buffer alignment to 4k boundary
  std::vector<cl::Device> devices;
  cl::Device device;
  if(setupDevice(devices, device) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  // Creating Context and Command Queue for selected device
  cl::Context context(device);
  cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE);

  // ======= Found Device =======================================================================================

  // Load rotation xclbin
  char *rxclbinFilename = argv[1];
  std::cout << "Loading: '" << rxclbinFilename << "'\n";
  std::ifstream rbin_file(rxclbinFilename, std::ifstream::binary);
  rbin_file.seekg(0, rbin_file.end);
  unsigned rnb = rbin_file.tellg();
  rbin_file.seekg(0, rbin_file.beg);
  char *rbuf = new char[rnb];
  rbin_file.read(rbuf, rnb);


  // Creating Program from Binary File
  cl::Program::Binaries bins;
  bins.push_back({rbuf, rnb});
  cl::Program program(context, devices, bins);

  // This call will get the kernel object from program. A kernel is an
  // OpenCL function that is executed on the FPGA.

  printf("INITIALIZING DATA\n");

  // float* in1 = new float[N_INPUT_1_1];
  // for(int i = 0; i < N_INPUT_1_1; i++){
  //   in1[i] = i * 2.346547; // some random decimal to produce some decimal results
  // }
  
  float* inX = new float[NHITS * NPARAMS];
  int* in_ei = new int[NEDGES * 2];

  float* out_y = new float[NEDGES];
  
  std::ifstream file("inputs.dat");
  if(file.is_open()){
    std::string sa;

    int dataset = 1; // which dataset to read from file, terribly efficiency wise; there are 3 (1,2,3)

    if(argc > 2){
      dataset = std::stod(argv[2]);
      std::cout << "read argv" << std::endl;
    }
    std::cout << "dataset: #" << dataset << std::endl;

    for(int d = 0; d < dataset; d++){
      for(int i = 0; i < NHITS * NPARAMS; i++){
        getline(file, sa);
        inX[i] = std::stof(sa);
        // in1[i] = i * 2.346547; // some random decimal to produce some decimal results
      }
      for(int i = 0; i < NEDGES * 2; i++){
        getline(file, sa);
        in_ei[i] = std::stoi(sa);
        // in1[i] = i * 2.346547; // some random decimal to produce some decimal results
      }
      std::cout << "True Outs" << std::endl;
      for(int i = 0; i < NEDGES; i++){
        getline(file, sa);
        out_y[i] = std::stof(sa);
        std::cout << sa << std::endl;
        // inRo[i] = std::stof(sa);
        // in1[i] = i * 2.346547; // some random decimal to produce some decimal results
      }
    }
  } else {
    for(int i = 0; i < NHITS * NPARAMS; i++){
      inX[i] = i * 2.34567;
    }

    for(int i = 0; i < NEDGES * 2; i++){
      in_ei[i] = (i%4==0) ? 1 : 0;
    }
  }

  float* out1 = new float[NEDGES];
  printf("\nRUNNING GRAPH\n\n");
  setupRunGraph(program, context, q, inX, in_ei, out1);

  std::cout << std::endl;

  printf("\nTrue Outs:\n");
  for (int i = 0; i < NEDGES; i++) {
    std::cout << out_y[i] << ", ";
  }

  std::cout << std::endl;

  printf("\n Diff True-Pred:\n");
  for (int i = 0; i < NEDGES; i++) {
    std::cout << out_y[i] - out1[i] << ", ";
  }

  std::cout << "Finished" << std::endl;
  return 0;
}
