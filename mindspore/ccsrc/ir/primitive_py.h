/**
 * Copyright 2019 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MINDSPORE_CCSRC_IR_PRIMITIVE_PY_H_
#define MINDSPORE_CCSRC_IR_PRIMITIVE_PY_H_

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <tuple>
#include <map>

#include "abstract/abstract_value.h"
#include "utils/misc.h"
#include "pybind11/pybind11.h"
#include "utils/log_adapter.h"
#include "ir/primitive.h"
#include "ir/signature.h"
#include "parallel/ops_info/operator_info.h"

namespace py = pybind11;
namespace mindspore {
class PrimitivePy : public Primitive {
 public:
  PrimitivePy(const py::str &name, const py::object &python_obj)
      : Primitive(name, false), python_obj_(python_obj), signatures_() {}
  ~PrimitivePy() override = default;
  MS_DECLARE_PARENT(PrimitivePy, Primitive);
  py::function GetBpropFunction();
  py::function GetComputeFunction();

  void set_signatures(
    std::vector<std::tuple<std::string, SignatureEnumRW, SignatureEnumKind, py::object, SignatureEnumDType>>
      signatures);

  const std::vector<Signature> &signatures() const { return signatures_; }

  void CopyHookFunction(const PrimitivePtr &primitive) override;

  void AddPyAttr(const py::str &name, const py::object &obj);

  py::dict GetAttrDict();
  void set_hook(const py::function &hook) { hook_ = hook; }
  py::function hook() const { return hook_; }
  BaseRef RunHookFunction(const VectorRef &args) const override;
  const bool parse_info_ = true;
  const py::object &GetPyObj() const { return python_obj_; }
  bool is_tuple_input_ = false;

 private:
  py::object python_obj_;
  py::function hook_;
  std::vector<Signature> signatures_;
  static std::map<std::string, py::object> hook_grad_;
};

using PrimitivePyPtr = std::shared_ptr<PrimitivePy>;
}  // namespace mindspore
#endif  // MINDSPORE_CCSRC_IR_PRIMITIVE_PY_H_