ROOT_DIR := $(realpath ../../..)
include $(ROOT_DIR)/config.mk

HIP_DIR := $(VORTEX_HOME)/sw/hip




CXX       := g++
AR        := ar
CXXFLAGS  := -Wall -Wextra -std=c++17 -I. -I../include
LDFLAGS += -L$(VORTEX_RT_PATH) -lvortex

VORTEX_RT_PATH ?= $(ROOT_DIR)/runtime
VORTEX_KN_PATH ?= $(ROOT_DIR)/kernel

CXXFLAGS += -I$(VORTEX_HOME)/runtime/include -I$(ROOT_DIR)/hw

CXXFLAGS += -I$(HIP_DIR)/include  -I$(HIP_DIR)/src

SRCS       := $(HIP_DIR)/src/hip.cpp
OBJS       := $(notdir $(SRCS:.cpp=.o))
LIB        := libvortexhip.a

all: $(LIB)

$(LIB): $(OBJS)
	@echo "AR  $@"
	ar rcs $@ $^

%.o: $(HIP_DIR)/src/%.cpp
	@echo "CXX $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -f $(OBJS) $(LIB)
