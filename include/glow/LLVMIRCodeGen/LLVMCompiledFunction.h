/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef GLOW_LLVMIRCODEGEN_LLVMCOMPILEDFUNCTION_H
#define GLOW_LLVMIRCODEGEN_LLVMCOMPILEDFUNCTION_H

#include "glow/LLVMIRCodeGen/GlowJIT.h"

#include "glow/Backends/BackendUtils.h"
#include "glow/Backends/CompiledFunction.h"

namespace glow {
/// A Glow IR function compiled using LLVM.
class LLVMCompiledFunction : public CompiledFunction {
public:
  LLVMCompiledFunction(std::unique_ptr<llvm::orc::GlowJIT> JIT,
                       const runtime::RuntimeBundle &runtimeBundle);

  /// \name CompiledFunction interface
  ///@{
  virtual ~LLVMCompiledFunction() override;
  virtual void execute(Context *ctx) override;

  virtual void collectConstants(Module *module) override;

  /// Read trace events out of this func and write them into /p ctx
  virtual void translateTraceEvents(Context *ctx) const override;
  ///@}
  //

protected:
  /// Load constant tensors from \p ctx into \p weightsAddress, as defined by
  /// the RuntimeBundle (pre-run).
  virtual void loadPlaceholders(Context *ctx, uint8_t *weightsAddress);

  /// Load weights from \p weightsAddress into applicable backing tensors in
  /// \p ctx, as defined by the RuntimeBundle (post-run).
  virtual void updatePlaceholders(Context *ctx, uint8_t *weightsAddress);

  /// The LLVM JIT engine. The jit must be initialized after the ctor
  /// initializes the LLVM backends.
  std::unique_ptr<llvm::orc::GlowJIT> JIT_;
};
} // end namespace glow

#endif // GLOW_LLVMIRCODEGEN_LLVMCOMPILEDFUNCTION_H
