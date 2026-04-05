//==============================================================================
// Auto Generated Code for MyScaleOpPackage
//==============================================================================
#include <cstring>
#include <iostream>
#include <string>

#include "CpuBackendUtils.hpp"
#include "CustomOpPackage.hpp"

using namespace qnn::custom;
using namespace qnn::custom::utils;

namespace myscale {

Qnn_ErrorHandle_t execute(CustomOp* operation) {
  QNN_CUSTOM_BE_ENSURE(operation != nullptr, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT);
  QNN_CUSTOM_BE_ENSURE_EQ(operation->numInput(), 1, QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);
  QNN_CUSTOM_BE_ENSURE_EQ(operation->numOutput(), 1, QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);

  // 取输入/输出 tensor
  auto inTensor  = reinterpret_cast<QnnCpuOpPackage_Tensor_t*>(operation->getInput(0));
  auto outTensor = reinterpret_cast<QnnCpuOpPackage_Tensor_t*>(operation->getOutput(0));

  QNN_CUSTOM_BE_ENSURE(inTensor != nullptr, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT);
  QNN_CUSTOM_BE_ENSURE(outTensor != nullptr, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT);

  // 这里只做最小单测，所以强制要求 float32
  QNN_CUSTOM_BE_ENSURE_EQ(inTensor->dataType, QNN_CPU_DATATYPE_FLOAT_32,
                          QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);
  QNN_CUSTOM_BE_ENSURE_EQ(outTensor->dataType, QNN_CPU_DATATYPE_FLOAT_32,
                          QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);

  // 输入输出元素个数必须一致
  const uint32_t numElems = numTensorSize(operation->getInput(0));
  QNN_CUSTOM_BE_ENSURE_EQ(numTensorSize(operation->getOutput(0)), numElems,
                          QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);

  const float* input = reinterpret_cast<const float*>(inTensor->data);
  float* output      = reinterpret_cast<float*>(outTensor->data);

  QNN_CUSTOM_BE_ENSURE(input != nullptr,  QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT);
  QNN_CUSTOM_BE_ENSURE(output != nullptr, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT);

  auto alphaParam = operation->getParam("alpha");
  QNN_CUSTOM_BE_ENSURE(alphaParam != nullptr, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT);

  auto cpuAlphaParam = reinterpret_cast<QnnCpuOpPackage_Param_t*>(alphaParam);
  QNN_CUSTOM_BE_ENSURE_EQ(cpuAlphaParam->type, QNN_CPU_PARAMTYPE_SCALAR,
                          QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);

  const float alpha = static_cast<float>(backend_utils::getScalarParam(alphaParam));

  // 核心计算：output = input * alpha
  for (uint32_t i = 0; i < numElems; ++i) {
    output[i] = input[i] * alpha;
  }

  return QNN_SUCCESS;
}

Qnn_ErrorHandle_t finalize(const CustomOp* operation) {
  QNN_CUSTOM_BE_ENSURE(operation != nullptr, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT);
  QNN_CUSTOM_BE_ENSURE_EQ(operation->numInput(), 1, QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);
  QNN_CUSTOM_BE_ENSURE_EQ(operation->numOutput(), 1, QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);

  auto inTensor  = reinterpret_cast<QnnCpuOpPackage_Tensor_t*>(operation->getInput(0));
  auto outTensor = reinterpret_cast<QnnCpuOpPackage_Tensor_t*>(operation->getOutput(0));

  QNN_CUSTOM_BE_ENSURE(inTensor != nullptr, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT);
  QNN_CUSTOM_BE_ENSURE(outTensor != nullptr, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT);

  // 最小例子：输入输出都要求 float32
  QNN_CUSTOM_BE_ENSURE_EQ(inTensor->dataType, QNN_CPU_DATATYPE_FLOAT_32,
                          QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);
  QNN_CUSTOM_BE_ENSURE_EQ(outTensor->dataType, QNN_CPU_DATATYPE_FLOAT_32,
                          QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);

  auto alphaParam = const_cast<CustomOp*>(operation)->getParam("alpha");
  QNN_CUSTOM_BE_ENSURE(alphaParam != nullptr, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT);

  auto cpuAlphaParam = reinterpret_cast<QnnCpuOpPackage_Param_t*>(alphaParam);
  QNN_CUSTOM_BE_ENSURE_EQ(cpuAlphaParam->type, QNN_CPU_PARAMTYPE_SCALAR,
                          QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE);

  return QNN_SUCCESS;
}

Qnn_ErrorHandle_t free(CustomOp& operation) {
  (void)operation;
  return QNN_SUCCESS;
}

Qnn_ErrorHandle_t populateFromNode(const QnnOpPackage_Node_t node,
                                   QnnOpPackage_GraphInfrastructure_t graphInfrastructure,
                                   CustomOp* operation) {
  // Add input
  for (uint32_t i = 0; i < numInputs(node); i++) {
    operation->addInput(getInput(node, i));
  }

  // Add output
  for (uint32_t i = 0; i < numOutputs(node); i++) {
    operation->addOutput(getOutput(node, i));
  }

  // Add params
  auto alphaPair = getParam(node, "alpha");
  QNN_CUSTOM_BE_ENSURE(alphaPair.first, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT)
  operation->addParam("alpha", alphaPair.second);

  return QNN_SUCCESS;
}

Qnn_ErrorHandle_t validateOpConfig(Qnn_OpConfig_t opConfig) {
  QNN_CUSTOM_BE_ENSURE_EQ(
      strcmp(opConfig.v1.typeName, "MyScale"), 0, QNN_OP_PACKAGE_ERROR_INVALID_ARGUMENT)

  QNN_CUSTOM_BE_ENSURE_EQ(opConfig.v1.numOfInputs, 1, QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE)
  QNN_CUSTOM_BE_ENSURE_EQ(opConfig.v1.numOfOutputs, 1, QNN_OP_PACKAGE_ERROR_VALIDATION_FAILURE)

  return QNN_SUCCESS;
}

}  // namespace myscale

CustomOpRegistration_t* register_MyscaleCustomOp() {
  using namespace myscale;
  static CustomOpRegistration_t MyscaleRegister = {
      execute, finalize, free, validateOpConfig, populateFromNode};
  return &MyscaleRegister;
}

REGISTER_OP(MyScale, register_MyscaleCustomOp);