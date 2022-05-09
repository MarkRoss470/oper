#pragma once
#include <memory>
#include "operValue.h"

std::shared_ptr<operValue> operatorAdd(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorSubtract(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorMultiply(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorDivide(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorGreater(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorLesser(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorGreaterEqual(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorLesserEqual(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorEquals(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> createOperator(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorLength(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorThrow(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorMessage(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> operatorExtype(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> castInt(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> castString(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> castFloat(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
std::shared_ptr<operValue> castBool(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1);
