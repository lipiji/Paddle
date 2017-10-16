/* Copyright (c) 2016 PaddlePaddle Authors. All Rights Reserve.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "paddle/operators/math/sequence2batch.h"

namespace paddle {
namespace operators {
namespace math {

template <typename T>
class CopyMatrixRowsFunctor<platform::CPUPlace, T> {
 public:
  void operator()(const platform::DeviceContext& context,
                  const framework::Tensor& src, const size_t* index,
                  framework::Tensor& dst, bool is_src_index) {
    auto src_dims = src.dims();
    auto dst_dims = dst.dims();
    PADDLE_ENFORCE(src_dims.size(), 2, "The src must be matrix with rank 2.");
    PADDLE_ENFORCE(dst_dims.size(), 2, "The dst must be matrix with rank 2.");
    PADDLE_ENFORCE_EQ(src_dims[1], dst_dims[1],
                      "The width of src and dst must be same.");
    auto height = dst_dims[0];
    auto width = dst_dims[1];
    auto* src_data = src.data<T>();
    auto* dst_data = dst.data<T>();
    for (int i = 0; i < height; ++i) {
      if (is_src_index) {
        memcpy(dst_data + i * width, src_data + index[i] * width,
               width * sizeof(T));
      } else {
        memcpy(dst_data + index[i] * width, src_data + i * width,
               width * sizeof(T));
      }
    }
  }
};

template class CopyMatrixRowsFunctor<platform::CPUPlace, float>;
template class CopyMatrixRowsFunctor<platform::CPUPlace, double>;

template class LoDTensor2BatchFunctor<platform::CPUPlace, float>;
template class Batch2LoDTensor2Functor<platform::CPUPlace, float>;

}  // namespace math
}  // namespace operators
}  // namespace paddle
