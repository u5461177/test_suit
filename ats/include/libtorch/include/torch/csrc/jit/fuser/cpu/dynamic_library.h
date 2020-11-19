#pragma once

#include <torch/csrc/jit/assertions.h>
#include <torch/csrc/utils/disallow_copy.h>

namespace torch {
namespace jit {
namespace fuser {
namespace cpu {

struct DynamicLibrary {
  TH_DISALLOW_COPY_AND_ASSIGN(DynamicLibrary);

  DynamicLibrary(const char* name);

  void* sym(const char* name);

  ~DynamicLibrary();

 private:
  void* handle = nullptr;
};

} // namespace cpu
} // namespace fuser
} // namespace jit
} // namespace torch
