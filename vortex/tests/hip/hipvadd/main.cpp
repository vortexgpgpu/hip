#define __HIP_PLATFORM_VORTEX__
#define __HIP_PLATFORM_AMD__ 


#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <vortex.h>
#include "common.h"

#include <vortexhip.h>

#define FLOAT_ULP 6

#ifdef NDEBUG
#define HIP_ASSERT(x) x
#else
#define HIP_ASSERT(x) (assert((x)==hipSuccess))
#endif

#define RT_CHECK(_expr)                                         \
   do {                                                         \
     int _ret = _expr;                                          \
     if (0 == _ret)                                             \
       break;                                                   \
     printf("Error: '%s' returned %d!\n", #_expr, (int)_ret);   \
	 cleanup();			                                              \
     exit(-1);                                                  \
   } while (false)



template <typename Type>
class Comparator {};

template <>
class Comparator<int> {
public:
  static const char* type_str() {
    return "integer";
  }
  static int generate() {
    return rand();
  }
  static bool compare(int a, int b, int index, int errors) {
    if (a != b) {
      if (errors < 100) {
        printf("*** error: [%d] expected=%d, actual=%d\n", index, b, a);
      }
      return false;
    }
    return true;
  }
};

template <>
class Comparator<float> {
private:
  union Float_t { float f; int i; };
public:
  static const char* type_str() {
    return "float";
  }
  static float generate() {
    return static_cast<float>(rand()) / RAND_MAX;
  }
  static bool compare(float a, float b, int index, int errors) {
    union fi_t { float f; int32_t i; };
    fi_t fa, fb;
    fa.f = a;
    fb.f = b;
    auto d = std::abs(fa.i - fb.i);
    if (d > FLOAT_ULP) {
      if (errors < 100) {
        printf("*** error: [%d] expected=%f, actual=%f\n", index, b, a);
      }
      return false;
    }
    return true;
  }
};


uint32_t size = 16;















static void show_usage() {
   std::cout << "Vortex Test." << std::endl;
   std::cout << "Usage: [-k: kernel] [-n words] [-h: help]" << std::endl;
}

static void parse_args(int argc, char **argv) {
  int c;
  while ((c = getopt(argc, argv, "n:k:h")) != -1) {
    switch (c) {
    case 'n':
      size = atoi(optarg);
      break;
    case 'k':
      kernel_file = optarg;
      break;
    case 'h':
      show_usage();
      exit(0);
      break;
    default:
      show_usage();
      exit(-1);
    }
  }
}

void cleanup() {
  if (device) {
    
    HIP_ASSERT(hipFree(src0_buffer));
    HIP_ASSERT(hipFree(src1_buffer));
    HIP_ASSERT(hipFree(dst_buffer));
    HIP_ASSERT(hipFree(krnl_buffer));
    HIP_ASSERT(hipFree(args_buffer));
    
    
    
  }
}

void 
vectoradd_float(float* __restrict__ a, const float* __restrict__ b, const float* __restrict__ c, int width, int height) 

  {
      int x = 1;
      int y = 1;

      int i = y * width + x;
      if ( i < (width * height)) {
        a[i] = b[i] + c[i];
      }

  }


int main(int argc, char *argv[]) {
  
  
  parse_args(argc, argv);

  std::srand(50);

  
  std::cout << "open device connection" << std::endl;
  
  

  uint32_t num_points = size;
  uint32_t buf_size = num_points * sizeof(TYPE);

  std::cout << "number of points: " << num_points << std::endl;
  std::cout << "data type: " << Comparator<TYPE>::type_str() << std::endl;
  std::cout << "buffer size: " << buf_size << " bytes" << std::endl;

  kernel_arg.num_points = num_points;

  
  std::cout << "allocate device memory" << std::endl;
  
  
  
  
  
  
  HIP_ASSERT(hipMalloc((void**)&src0_buffer,buf_size));
  HIP_ASSERT(hipMalloc((void**)&src1_buffer,buf_size));
  HIP_ASSERT(hipMalloc((void**)&dst_buffer,buf_size));

  std::cout << "dev_src0=0x" << std::hex << kernel_arg.src0_addr << std::endl;
  std::cout << "dev_src1=0x" << std::hex << kernel_arg.src1_addr << std::endl;
  std::cout << "dev_dst=0x" << std::hex << kernel_arg.dst_addr << std::endl;

  
  std::cout << "allocate host buffers" << std::endl;
  std::vector<TYPE> h_src0(num_points);
  std::vector<TYPE> h_src1(num_points);
  std::vector<TYPE> h_dst(num_points);

  for (uint32_t i = 0; i < num_points; ++i) {
    h_src0[i] = Comparator<TYPE>::generate();
    h_src1[i] = Comparator<TYPE>::generate();
  }

  
  std::cout << "upload source buffer0" << std::endl;
  
  HIP_ASSERT(hipMemcpy(src0_buffer, h_src0.data(), buf_size, hipMemcpyHostToDevice));

  
  std::cout << "upload source buffer1" << std::endl;
  
  HIP_ASSERT(hipMemcpy(src1_buffer, h_src1.data(), buf_size, hipMemcpyHostToDevice));

  
  
  

  
  
  

  
  
  

  
  
  

  
  hipLaunchKernelGGL(vectoradd_float, 
                  mydim3(3, 3),
                  mydim3(3,3),
                  0, 0,src0_buffer,src1_buffer ,dst_buffer ,3,3);






  
  std::cout << "download destination buffer" << std::endl;
  
  HIP_ASSERT(hipMemcpy(h_dst.data(), dst_buffer, buf_size, hipMemcpyDeviceToHost));





  
  

  
  std::cout << "verify result" << std::endl;
  int errors = 0;
  for (uint32_t i = 0; i < num_points; ++i) {
    auto ref = h_src0[i] + h_src1[i];
    auto cur = h_dst[i];
    if (!Comparator<TYPE>::compare(cur, ref, i, errors)) {
      ++errors;
    }
  }

  
  std::cout << "cleanup" << std::endl;
  cleanup();

  if (errors != 0) {
    std::cout << "Found " << std::dec << errors << " errors!" << std::endl;
    std::cout << "FAILED!" << std::endl;
    return 1;
  }

  std::cout << "PASSED!" << std::endl;

  return 0;
}







/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
// #include <assert.h>
// #include <stdio.h>
// #include <algorithm>
// #include <stdlib.h>
// #include<iostream>
// #include "hip/hip_runtime.h"


// #ifdef NDEBUG
// #define HIP_ASSERT(x) x
// #else
// #define HIP_ASSERT(x) (assert((x)==hipSuccess))
// #endif


// #define WIDTH     1024
// #define HEIGHT    1024

// #define NUM       (WIDTH*HEIGHT)

// #define THREADS_PER_BLOCK_X  16
// #define THREADS_PER_BLOCK_Y  16
// #define THREADS_PER_BLOCK_Z  1

// __global__ void 
// vectoradd_float(float* __restrict__ a, const float* __restrict__ b, const float* __restrict__ c, int width, int height) 

//   {
 
//       int x = hipBlockDim_x * hipBlockIdx_x + hipThreadIdx_x;
//       int y = hipBlockDim_y * hipBlockIdx_y + hipThreadIdx_y;

//       int i = y * width + x;
//       if ( i < (width * height)) {
//         a[i] = b[i] + c[i];
//       }



//   }

// #if 0
// __kernel__ void vectoradd_float(float* a, const float* b, const float* c, int width, int height) {

  
//   int x = blockDimX * blockIdx.x + threadIdx.x;
//   int y = blockDimY * blockIdy.y + threadIdx.y;

//   int i = y * width + x;
//   if ( i < (width * height)) {
//     a[i] = b[i] + c[i];
//   }
// }
// #endif

// using namespace std;

// int main() {
  
//   float* hostA;
//   float* hostB;
//   float* hostC;

//   float* deviceA;
//   float* deviceB;
//   float* deviceC;
////////
  // hipDeviceProp_t devProp;
  // hipGetDeviceProperties(&devProp, 0);
  // cout << " System minor " << devProp.minor << endl;
  // cout << " System major " << devProp.major << endl;
  // cout << " agent prop name " << devProp.name << endl;



  // cout << "hip Device prop succeeded " << endl ;

//////////////
//   int i;
//   int errors;

//   hostA = (float*)malloc(NUM * sizeof(float));
//   hostB = (float*)malloc(NUM * sizeof(float));
//   hostC = (float*)malloc(NUM * sizeof(float));
  
//   // initialize the input data
//   for (i = 0; i < NUM; i++) {
//     hostB[i] = (float)i;
//     hostC[i] = (float)i*100.0f;
//   }
  
//   HIP_ASSERT(hipMalloc((void**)&deviceA, NUM * sizeof(float)));
//   HIP_ASSERT(hipMalloc((void**)&deviceB, NUM * sizeof(float)));
//   HIP_ASSERT(hipMalloc((void**)&deviceC, NUM * sizeof(float)));
  
//   HIP_ASSERT(hipMemcpy(deviceB, hostB, NUM*sizeof(float), hipMemcpyHostToDevice));
//   HIP_ASSERT(hipMemcpy(deviceC, hostC, NUM*sizeof(float), hipMemcpyHostToDevice));


//   hipLaunchKernelGGL(vectoradd_float, 
//                   dim3(WIDTH/THREADS_PER_BLOCK_X, HEIGHT/THREADS_PER_BLOCK_Y),
//                   dim3(THREADS_PER_BLOCK_X, THREADS_PER_BLOCK_Y),
//                   0, 0,
//                   deviceA ,deviceB ,deviceC ,WIDTH ,HEIGHT);


//   HIP_ASSERT(hipMemcpy(hostA, deviceA, NUM*sizeof(float), hipMemcpyDeviceToHost));

//   // verify the results
//   errors = 0;
//   for (i = 0; i < NUM; i++) {
//     if (hostA[i] != (hostB[i] + hostC[i])) {
//       errors++;
//     }
//   }
//   if (errors!=0) {
//     printf("FAILED: %d errors\n",errors);
//   } else {
//       printf ("PASSED!\n");
//   }

//   HIP_ASSERT(hipFree(deviceA));
//   HIP_ASSERT(hipFree(deviceB));
//   HIP_ASSERT(hipFree(deviceC));

//   free(hostA);
//   free(hostB);
//   free(hostC);

//   //hipResetDefaultAccelerator();

//   return errors;
// }
















































 















  













  








  
  
  
  
  



  








  





  



  





































