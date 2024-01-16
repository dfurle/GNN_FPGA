#ifndef MYPROJECT_H_INPUT
#define MYPROJECT_H_INPUT

#include "projectDefines.h"
#include "defines_i.h"

extern "C"{

// Prototype of top level function for C-synthesis
void input_network(data_t X[NHITS * NPARAMS], par_t H[NHITS]);

}

#endif
