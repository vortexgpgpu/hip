#define __HIP_PLATFORM_VORTEX__
#define __HIP_PLATFORM_AMD__ 

// #include <vortex.h>
#include "hip/hip_runtime.h"
#include <unistd.h>
#include <string.h>
#include <vector>
#include <iostream>
#include "vortexhip.h"

#undef hipDeviceProp_t
#undef hipGetDeviceProperties
#undef hipChooseDevice

// now, tell the compiler that everywhere *after* this point…
#define hipDeviceProp_t            hipDeviceProp_tVortex
#define hipGetDeviceProperties     hipGetDevicePropertiesVortex
#define hipChooseDevice            hipChooseDeviceVortex

#define RT_CHECK(_expr)                                         \
   do {                                                         \
     int _ret = _expr;                                          \
     if (0 == _ret)                                             \
       break;                                                   \
     printf("Error: '%s' returned %d!\n", #_expr, (int)_ret);   \
     exit(-1);                                                  \
   } while (false)



void HipVortexDebug() {
  std::cout << "HipVortexDebug" << std::endl;
}
// test
int hipDeviceProp_tVortex::test() {
  
    std::cout << "test" << std::endl;
    // return vx_dev_open(&device);
}

int testopen(hipDeviceProp_tVortex* device){
  std::cout << "testopen" << std::endl;
  return vx_dev_open(&device->getdevice());
}





vx_device_h device = nullptr; 
kernel_arg_t kernel_arg = {};
vx_buffer_h krnl_buffer = nullptr;
vx_buffer_h args_buffer = nullptr;
const char* kernel_file = "kernel.vxbin";
vx_buffer_h src0_buffer = nullptr;
vx_buffer_h src1_buffer = nullptr;
vx_buffer_h dst_buffer = nullptr;

int flag = 0;
int close_flag = 1;


void helper_open() {
    std::cout << "opening device in helper" << std::endl;
    RT_CHECK(vx_dev_open(&device));
}


hipError_t hipMalloc(void** ptr, size_t size){
  std::cout << "hipMalloc from helper flag: " << flag << std::endl;
  try{
    if (flag == 0){
      helper_open();
      RT_CHECK(vx_mem_alloc(device, size, VX_MEM_READ, (vx_buffer_h*)ptr));
      RT_CHECK(vx_mem_address((vx_buffer_h)*ptr, &kernel_arg.src0_addr));
      flag = 1;
    }else if (flag == 1){
      RT_CHECK(vx_mem_alloc(device, size, VX_MEM_READ, (vx_buffer_h*)ptr));
      RT_CHECK(vx_mem_address((vx_buffer_h)*ptr, &kernel_arg.src1_addr));
      flag = 2;
    }else if (flag == 2){
      RT_CHECK(vx_mem_alloc(device, size, VX_MEM_WRITE, (vx_buffer_h*)ptr));
      RT_CHECK(vx_mem_address((vx_buffer_h)*ptr, &kernel_arg.dst_addr));
      flag += 3;
    }

  }
  catch (std::exception& e){
    std::cout << "hipMalloc exception" << std::endl;
    return hipErrorOutOfMemory;
  }
  

  return hipSuccess;
}


hipError_t hipMemcpy(void* dst, const void* src, size_t sizeBytes, hipMemcpyKind kind){
  try{
      if (kind == hipMemcpyHostToDevice){
      RT_CHECK(vx_copy_to_dev((vx_buffer_h)dst, src, 0, sizeBytes));
    }
    else if (kind == hipMemcpyDeviceToHost){
      RT_CHECK(vx_copy_from_dev(dst, (vx_buffer_h)src, 0, sizeBytes));
    }
    else{
      std::cout << "hipMemcpy: invalid kind" << std::endl;
      return hipErrorInvalidValue;
    }

  }
  catch (std::exception& e){
    std::cout << "hipMemcpy exception" << std::endl;
    return hipErrorUnknown;
  }
  return hipSuccess;
}

hipError_t hipFree(void* ptr){
  try{
    std::cout << "hipFree" << std::endl;
    RT_CHECK(vx_mem_free((vx_buffer_h)ptr));
    if (close_flag == flag){
      vx_dev_close(device);
      std::cout << "Device closed" << std::endl;
    }
    close_flag++;
    return hipSuccess;
  }catch (std::exception& e){
    std::cout << "hipFree exception" << std::endl;
    return hipErrorInvalidValue;
  }
}



