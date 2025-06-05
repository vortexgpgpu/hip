ROOT_DIR := $(realpath ../../..)
include $(ROOT_DIR)/config.mk

PROJECT := hipvadd

SRC_DIR := $(VORTEX_HOME)/tests/hip/$(PROJECT)

HIP_DIR := $(VORTEX_HOME)/sw/hip

SRCS := $(SRC_DIR)/main.cpp #$(HIP_DIR)/src/hip.cpp

VX_SRCS := $(SRC_DIR)/kernel.cpp

CXXFLAGS += -I$(HIP_DIR)/include  -I$(HIP_DIR)/src

VX_CFLAGS += -I$(HIP_DIR)/include  -I$(HIP_DIR)/src

OPTS ?= -n64

include ../common.mk