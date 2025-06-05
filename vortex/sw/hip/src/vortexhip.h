#ifndef _VORTEXHIP_H_
#define _VORTEXHIP_H_

#include <hip/hip_runtime.h>      
#include <vortex.h>     
#include <cstddef>        
#include <iostream>

#undef hipDeviceProp_t
#undef hipGetDeviceProperties
#undef hipChooseDevice

// 3) Remap them to your Vortex versions
#define hipDeviceProp_t            hipDeviceProp_tVortex
#define hipGetDeviceProperties     hipGetDevicePropertiesVortex
#define hipChooseDevice            hipChooseDeviceVortex




struct mydim3 {
    uint32_t x, y, z;

    // Default to (1,1,1) so that a single-argument or no-argument ctor still makes sense
    mydim3(uint32_t x_ = 1,
                   uint32_t y_ = 1,
                   uint32_t z_ = 1) noexcept
      : x(x_), y(y_), z(z_) {}

};



class hipDeviceProp_tVortex {
public:
    // ctor/dtor
    hipDeviceProp_tVortex();
    ~hipDeviceProp_tVortex();

    // singleton accessor
    static hipDeviceProp_tVortex& getInstance();


    // simple test helper
    int test();

    vx_device_h& getdevice(){
        return device;
    };


private:
    vx_device_h device;
};


#ifndef KERNEL_ARG_T_DEFINED
#define KERNEL_ARG_T_DEFINED

typedef struct {
  uint32_t num_points;
  uint64_t src0_addr;
  uint64_t src1_addr;
  uint64_t dst_addr;  
} kernel_arg_t;

#endif

int testopen(hipDeviceProp_tVortex* hdevice);

int new_dev_open(hipDeviceProp_tVortex* p);

extern const char* kernel_file;
extern vx_device_h device;
extern kernel_arg_t kernel_arg;
extern vx_buffer_h krnl_buffer;
extern vx_buffer_h args_buffer;
extern vx_buffer_h src0_buffer;
extern vx_buffer_h src1_buffer;
extern vx_buffer_h dst_buffer;

template <typename... Args, typename F = void (*)(Args...)>
inline
void hipLaunchKernelGGL(F kernel, const mydim3& numBlocks, const mydim3& dimBlocks,
                        std::uint32_t sharedMemBytes, hipStream_t stream,
                        Args... args) {


  std::cout << "hipLaunchKernelGGL" << std::endl;
std::cout << "upload program" << std::endl;
  vx_upload_kernel_file(device, kernel_file, &krnl_buffer);

  // upload kernel argument
  std::cout << "upload kernel argument" << std::endl;
  vx_upload_bytes(device, &kernel_arg, sizeof(kernel_arg_t), &args_buffer);

  // start device
  std::cout << "start device" << std::endl;
  vx_start(device, krnl_buffer, args_buffer);

  // wait for completion
  std::cout << "wait for completion" << std::endl;
  vx_ready_wait(device, VX_MAX_TIMEOUT);



                        }



void helper_open();

#endif