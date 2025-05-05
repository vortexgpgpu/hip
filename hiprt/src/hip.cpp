#include <vortex.h>

class HipDevice {
public:
  // Constructor
  HipDevice() {
    // Initialize the device
    RT_CHECK(vx_dev_open(&vx_device_));
  }

  ~HipDevice() {
    vx_dev_close(vx_device_);
  }

  // create singleton accessor
  static HipDevice& getInstance() {
    static HipDevice instance;
    return instance;
  }

  hipError_t allocate(void** ptr, std::size_t size) {
    vx_buffer_h buffer;
    int ret = vx_mem_alloc(vx_device_, size, 0, &buffer);
    if (ret != 0) {
      return hipErrorMemoryAllocation;
    }
    *ptr = reinterpret_cast<void*>(buffer);
    return hipSuccess;
  }

private:
  vx_device_h vx_device_;
};

hipError_t hipMalloc(void** ptr, std::size_t size) {
  return HipDevice::getInstance().allocate(ptr, size);
}

hipError_t hipMemcpy(
    void* dst,
    const void* src,
    std::size_t size,
    hipMemcpyKind kind = hipMemcpyDefault)
{
  // TODO: HipDevice::getInstance().mempcy(dst, src, size, kind);
  return hipErrorNotSupported;
}

hipError_t hipMemcpy(
    void* dst,
    const void* src,
    std::size_t size,
    hipMemcpyKind kind = hipMemcpyDefault)
{
  // TODO: HipDevice::getInstance().mempcy(dst, src, size, kind);
  return hipErrorNotSupported;
}

hipError_t hipModuleLaunchKernel(hipFunction_t f, unsigned int gridDimX, unsigned int gridDimY,
                                 unsigned int gridDimZ, unsigned int blockDimX,
                                 unsigned int blockDimY, unsigned int blockDimZ,
                                 unsigned int sharedMemBytes, hipStream_t stream,
                                 void** kernelParams, void** extra) {
 // TODO:
 return hipErrorNotSupported;
 }