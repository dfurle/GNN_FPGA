
# name of .xcl file
HLS_NAME := GraphNetwork
# xilinx part to be used
# PART := xilinx_u250_gen3x16_xdma_4_1_202210_1
# PART := xilinx_vmk180_base_202310_1
# emulation mode (sw_emu, hw_emu, hw)
EMU_MODE := sw_emu

########

WORK := $(realpath ../)
LDFLAGS := -L${XILINX_XRT}/lib
LDFLAGS += -lxilinxopencl
# LDFLAGS += -pthread   #doesnt seem to be needed, but if anything breaks try uncomment this first!


INPUT_DIR := ${WORK}/network/input
INPUT_SRC := ${INPUT_DIR}/myproject_i.cpp
# INPUT_INCLUDE := -I${INPUT_DIR}
	
EDGE_DIR := ${WORK}/network/edge
EDGE_SRC := ${EDGE_DIR}/myproject_e.cpp
# EDGE_INCLUDE := -I${EDGE_DIR}

NODE_DIR := ${WORK}/network/node
NODE_SRC := ${NODE_DIR}/myproject_n.cpp
# NODE_INCLUDE := -I${NODE_DIR}

RUNNER_SRC := ${WORK}/runner/runner.cpp ${INPUT_SRC} ${NODE_SRC} ${EDGE_SRC}
# RUNNER_INCLUDE := -I${WORK} -I${WORK}/network/firmware ${INPUT_INCLUDE} ${EDGE_INCLUDE} ${NODE_INCLUDE}
RUNNER_KERNEL := runGraphNetwork
# KERNEL_CFG := ${WORK}/runner/hls_config.cfg
KERNEL_CFG := ${WORK}/FPGA_MODEL/hls_config.cfg

main: ${RUNNER_KERNEL}.zip

.PHONY: host compile link clean all

all: compile

${RUNNER_KERNEL}.zip: ${RUNNER_SRC} ${KERNEL_CFG}
	mkdir -p ./kernels
	cd ./kernels; \
	v++ -c --mode hls --config ${KERNEL_CFG}; \
	mv rungraphnetwork/${RUNNER_KERNEL}.zip ..

csim: ${RUNNER_SRC} ${KERNEL_CFG}
	vitis-run --mode hls --csim --config ${KERNEL_CFG} --work_dir ./kernels/runner

compile:
	${RUNNER_KERNEL}.zip
	${MAKE}

clean:
	rm -rf ./.Xil/
	rm -rf ./.run/
	rm -rf ./kernels
	rm -rf ./xclbin
	rm -f *.xo
	rm -f *.zip
	rm -f *.xclbin
	rm -f *.exe
	rm -f emconfig.json
	rm -f export.sh
	rm -f host.exe

