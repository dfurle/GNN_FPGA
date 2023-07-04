#ifndef MYPROJECT_H_
#define MYPROJECT_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_stream.h"

#include "projectDefines.h"

#include "defines.h"

extern "C"{

// Prototype of top level function for C-synthesis
// void input_network(data_t X[NHITS * NPARAMS], data_t H[NHITS * NPARHID]);
void input_network(data_t X[NHITS * NPARAMS], data_t Ro[NHITS * NEDGES], data_t Ri[NHITS * NEDGES], data_t H[NHITS * NPARHID], data_t e[NEDGES]);

}

#endif
