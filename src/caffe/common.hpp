#ifndef CAFFE_COMMON_HPP_
#define CAFFE_COMMON_HPP_

#include <boost/shared_ptr.hpp>
#include <cublas_v2.h>
#include <cuda.h>
#include <curand.h>
#include <glog/logging.h>
#include <mkl_vsl.h>

#include "driver_types.h"

#define CUDA_CHECK(condition) CHECK_EQ((condition), cudaSuccess)
#define CUBLAS_CHECK(condition) CHECK_EQ((condition), CUBLAS_STATUS_SUCCESS)
#define CURAND_CHECK(condition) CHECK_EQ((condition), CURAND_STATUS_SUCCESS)
#define VSL_CHECK(condition) CHECK_EQ((condition), VSL_STATUS_OK)

#define CUDA_POST_KERNEL_CHECK \
  if (cudaSuccess != cudaPeekAtLastError()) {\
    LOG(FATAL) << "Cuda kernel failed. Error: " << cudaGetLastError(); \
  }

#define INSTANTIATE_CLASS(classname) \
  template class classname<float>; \
  template class classname<double>

#define NOT_IMPLEMENTED LOG(FATAL) << "Not Implemented Yet"

namespace caffe {

// We will use the boost shared_ptr instead of the new C++11 one mainly
// because cuda does not work (at least now) well with C++11 features.
using boost::shared_ptr;

// For backward compatibility we will just use 512 threads per block
const int CAFFE_CUDA_NUM_THREADS = 512;

inline int CAFFE_GET_BLOCKS(const int N) {
  return (N + CAFFE_CUDA_NUM_THREADS - 1) / CAFFE_CUDA_NUM_THREADS;
}

// A singleton class to hold common caffe stuff, such as the handler that
// caffe is going to use for cublas.
class Caffe {
 public:
  ~Caffe();
  static Caffe& Get();
  enum Brew { CPU, GPU };
  enum Phase { TRAIN, TEST};

  // The getters for the variables. 
  static cublasHandle_t cublas_handle();
  static curandGenerator_t curand_generator();
  static VSLStreamStatePtr vsl_stream();
  static Brew mode();
  static Phase phase();
  // The setters for the variables
  static void set_mode(Brew mode);
  static void set_phase(Phase phase);
  static void set_random_seed(const unsigned int seed);
 private:
  Caffe();
  static shared_ptr<Caffe> singleton_;
  cublasHandle_t cublas_handle_;
  curandGenerator_t curand_generator_;
  VSLStreamStatePtr vsl_stream_;
  Brew mode_;
  Phase phase_;
};

}  // namespace caffe

#endif  // CAFFE_COMMON_HPP_