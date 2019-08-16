#include <torch/csrc/jit/hooks_for_testing.h>
#include <torch/csrc/jit/script/module.h>

#include <utility>

namespace torch {
namespace jit {

static ModuleHook emit_module_callback;
void didFinishEmitModule(const script::Module& module) {
  if (emit_module_callback) {
// [serialization forward compat]
#ifndef FBCODE_CAFFE2
    emit_module_callback(module);
#endif
  }
}

static FunctionHook emit_function_callback;
void didFinishEmitFunction(StrongFunctionPtr fn) {
  if (emit_function_callback) {
    emit_function_callback(std::move(fn));
  }
}

void setEmitHooks(ModuleHook for_mod, FunctionHook for_fn) {
  emit_module_callback = std::move(for_mod);
  emit_function_callback = std::move(for_fn);
}

std::pair<ModuleHook, FunctionHook> getEmitHooks() {
  return std::make_pair(emit_module_callback, emit_function_callback);
}

} // namespace jit
} // namespace torch
