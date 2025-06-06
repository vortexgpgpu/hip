/*
Copyright (c) 2015 - 2021 Advanced Micro Devices, Inc. All rights reserved.

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

//! HIP = Heterogeneous-compute Interface for Portability
//!
//! Define a extremely thin runtime layer that allows source code to be compiled unmodified
//! through either AMD CLANG or NVCC.   Key features tend to be in the spirit
//! and terminology of CUDA, but with a portable path to other accelerators as well:
//
//! Both paths support rich C++ features including classes, templates, lambdas, etc.
//! Runtime API is C
//! Memory management is based on pure pointers and resembles malloc/free/copy.
//
//! hip_runtime.h     : includes everything in hip_api.h, plus math builtins and kernel launch
//! macros. hip_runtime_api.h : Defines HIP API.  This is a C header file and does not use any C++
//! features.

#ifndef HIP_INCLUDE_HIP_HIP_RUNTIME_H
#define HIP_INCLUDE_HIP_HIP_RUNTIME_H

#if __HIP_DEVICE_COMPILE__ && !__GFX7__ && !__GFX8__ && !__GFX9__ && __AMDGCN_WAVEFRONT_SIZE == 64
#error HIP is not supported on the specified GPU ARCH with wavefront size 64
#endif

#if !defined(__HIPCC_RTC__)
// Some standard header files, these are included by hc.hpp and so want to make them avail on both
// paths to provide a consistent include env and avoid "missing symbol" errors that only appears
// on NVCC path:
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#if __cplusplus > 199711L
#include <thread>
#endif
#endif // !defined(__HIPCC_RTC__)

// #include <hip/hip_version.h> //vortex
#include <hip/hip_common.h>

#if defined(__HIP_PLATFORM_AMD__) && !defined(__HIP_PLATFORM_NVIDIA__) && !defined(__HIP_PLATFORM_VORTEX__)
#include <hip/amd_detail/amd_hip_runtime.h>
#elif !defined(__HIP_PLATFORM_AMD__) && defined(__HIP_PLATFORM_NVIDIA__)
#include <hip/nvidia_detail/nvidia_hip_runtime.h>
#elif defined(__HIP_PLATFORM_AMD__) && !defined(__HIP_PLATFORM_NVIDIA__) && defined(__HIP_PLATFORM_VORTEX__)
#include <vortex.h>
#else
#error("Must define exactly one of __HIP_PLATFORM_AMD__ or __HIP_PLATFORM_NVIDIA__");
#endif

#if !defined(__HIPCC_RTC__)
#include <hip/hip_runtime_api.h>
#include <hip/library_types.h>
#endif // !defined(__HIPCC_RTC__)
#include <hip/hip_vector_types.h>

#endif
