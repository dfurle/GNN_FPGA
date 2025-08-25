#pragma once
#include "projectDefines.h"
#include "defines_i.h"

extern "C"{
  void input_network(data_t X[NHITS * NPARAMS], par_t H[NHITS]);
}

