#pragma once

#include <c10/core/Tensor.h>
#include <c10/util/Array.h>
#include "caffe2/core/context_base.h"

namespace caffe2 {
namespace ops {

struct Cast final {
  static constexpr const char* name = "cast";

  using Signature = void(
      const C10Tensor& input1,
      const C10Tensor& output,
      TensorProto_DataType to);

  static constexpr size_t num_dispatch_args() {return 1;}

  static constexpr size_t num_outputs() {return 1;}

  static constexpr c10::guts::array<const char*, 3> parameter_names = {
      {"input", "output", "to"}};
};

} // namespace ops
} // namespace caffe2
