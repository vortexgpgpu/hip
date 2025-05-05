ROOT_DIR := $(realpath ../../..)
include $(ROOT_DIR)/config.mk

HIPCC := ?

HIPRT := ?

VORTEX_HOME := ?

CXXFLAGS := -std=c++11 -O2 -I

PROJECT := vadd

SRC_DIR := $(VORTEX_HOME)/tests/regression/$(PROJECT)

HIP_DIR := $(VORTEX_HOME)/sw/hip

RUNTIME_SRCS := $(HIP_DIR)/src/hip.cpp

SRCS := $(SRC_DIR)/main.cpp

CXXFLAGS += -I$(HIP_DIR)/include

VX_SRCS := $(SRC_DIR)/kernel.cpp

OPTS ?= -n64

.PHONY: all clean

all: $(PROJECT)

$(PROJECT): $(SRC)
	$(HIPCC) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGET)