#pragma once
#include "parser.h"
#include "bigint.h"
#include <unordered_map>

class operValue
{
    public:
        ~operValue();
        std::string toString();
        std::string type;
        std::shared_ptr<operValue> getVariable(std::string name);
        void setVariable(std::string name, std::shared_ptr<operValue> value);
        std::shared_ptr<operValue> evalOperator(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1, std::shared_ptr<operValue> scope);
        union
        {
            std::string string_value;
            BigInteger int_value;
            double float_value;
            bool bool_value;
            struct {
                std::shared_ptr<ASTNode> block;
                std::shared_ptr<operValue> parent;
                std::unordered_map<std::string, std::shared_ptr<operValue>> variables;
            } block_value;
            struct
            {
                std::shared_ptr<ASTNode> node;
                std::shared_ptr<operValue> scope;
            } reference_value;
            struct {
                std::shared_ptr<operValue> block;
                std::string arg0type;
                std::string arg1type;
            } operator_value;
            struct{
                std::string name;
                std::string arg0type;
                std::string arg1type;
            } builtin_operator_value;
            std::shared_ptr<operValue> return_value;
            struct{
                std::string type;
                std::string message;
                std::vector<std::tuple<uintmax_t, uintmax_t, std::string>> trace;
            } exception_value;
        };
};

#define operValueNone() operValue{.type="None"}
#define operValueString(X) operValue{.type="String", .string_value=X}
#define operValueInt(X) operValue{.type="Int", .int_value=X}
#define operValueFloat(X) operValue{.type="Float", .float_value=X}
#define operValueBool(X) operValue{.type="Bool", .bool_value=X}
#define operValueBlockCopy(X) operValue{.type="Block", .block_value={.block=X->block_value.block, .parent=X->block_value.parent, .variables=X->block_value.variables}}
#define operValueBlock(X,Y) operValue{.type="Block", .block_value={.block=X, .parent=Y, .variables=std::unordered_map<std::string, std::shared_ptr<operValue>>{}}}
#define operValueReference(X,Y) operValue{.type="Reference", .reference_value={.node=X, .scope=Y}}
#define operValueOperator(X,Y,Z) operValue{.type="Operator", .operator_value={.block=X, .arg0type=Y, .arg1type=Z}}
#define operValueBuiltinOperator(X,Y,Z) operValue{.type="BuiltinOperator", .builtin_operator_value={.name=X, .arg0type=Y, .arg1type=Z}}
#define operValueReturn(X) operValue{.type="ReturnValue", .return_value=X}
#define operValueException(X,Y) operValue{.type="Exception", .exception_value={.type=X, .message=Y, .trace={}}}


