/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "tensorflow/core/data/service/snapshot/utils.h"

#include <cstdint>
#include <vector>

#include "absl/strings/match.h"
#include "absl/strings/string_view.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/tensor.pb.h"
#include "tensorflow/tsl/platform/errors.h"
#include "tensorflow/tsl/platform/status.h"

namespace tensorflow {
namespace data {

int64_t EstimatedSizeBytes(const std::vector<Tensor>& tensors) {
  int64_t size_bytes = 0;
  for (const Tensor& tensor : tensors) {
    TensorProto proto;
    tensor.AsProtoTensorContent(&proto);
    size_bytes += proto.ByteSizeLong();
  }
  return size_bytes;
}

Status StreamAssignmentChanged(absl::string_view worker_address,
                               int64_t stream_index) {
  return errors::FailedPrecondition(
      "Worker ", worker_address,
      " has an outdated stream assignment: ", stream_index,
      ". It must heartbeat to the dispatcher to refresh its assigned stream.");
}

bool IsStreamAssignmentChanged(const Status& status) {
  return errors::IsFailedPrecondition(status) &&
         absl::StrContains(status.error_message(),
                           "has an outdated stream assignment");
}

}  // namespace data
}  // namespace tensorflow
