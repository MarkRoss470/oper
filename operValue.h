#pragma once
#include "parser.h"
#include "bigint.h"
#include <unordered_map>

enum class operValueType
{
    Int,
    Float,
    String,
    Bool,
    None,
    Block,
    Statement,
    Reference,
    Operator,
    BuiltinOperator,
    ReturnValue,
    Exception
};

class operValue
{
    public:
        //None
        inline operValue() : type(operValueType::None) {}
        //String
        inline operValue(std::string s) : type(operValueType::String), string_value(s) {}
        //Int
        inline operValue(BigInteger i) : type(operValueType::Int), int_value(i) {}
        //Float
        inline operValue(double d) : type(operValueType::Float), float_value(d) {}
        //Bool
        inline operValue(bool b) : type(operValueType::Bool), bool_value(b) {}
        //Block
        inline operValue(std::shared_ptr<ASTNode> b, std::shared_ptr<operValue> p, std::unordered_map<std::string, std::shared_ptr<operValue>> v) : type(operValueType::Block), block_value{.block=b, .parent=p, .variables=v} {}
        //Reference
        inline operValue(std::shared_ptr<ASTNode> n, std::shared_ptr<operValue> s) : type(operValueType::Reference), reference_value{.node=n, .scope=s} {}
        //Operator
        inline operValue(std::shared_ptr<operValue> b, std::string a0, std::string a1) : type(operValueType::Operator), operator_value{.block=b, .arg0type=a0, .arg1type=a1} {}
        //BuiltinOperator
        inline operValue(std::string n, std::string a0, std::string a1) : type(operValueType::BuiltinOperator), builtin_operator_value{.name=n, .arg0type=a0, .arg1type=a1} {}
        //ReturnValue
        inline operValue(std::shared_ptr<operValue> r) : type(operValueType::ReturnValue), return_value(r) {}
        //Exception
        inline operValue(std::string t, std::string m, std::vector<std::tuple<uintmax_t, uintmax_t, std::string>> tr) : type(operValueType::Exception), exception_value{.type=t, .message=m, .trace=tr} {}
        
        ~operValue();
        std::string toString();
        operValueType type;
        std::string getType();
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

#define operValueNone()
#define operValueString(X) (std::string)(X)
#define operValueInt(X) (BigInteger)(X)
#define operValueFloat(X) (double)(X)
#define operValueBool(X) (bool)(X)
#define operValueBlockCopy(X) X->block_value.block, X->block_value.parent, X->block_value.variables
#define operValueBlock(X,Y) (std::shared_ptr<ASTNode>)(X), (std::shared_ptr<operValue>)(Y), std::unordered_map<std::string, std::shared_ptr<operValue>>{}
#define operValueReference(X,Y) (std::shared_ptr<ASTNode>)(X), (std::shared_ptr<operValue>)(Y)
#define operValueOperator(X,Y,Z) (std::shared_ptr<operValue>)(X), (std::string)(Y), (std::string)(Z)
#define operValueBuiltinOperator(X,Y,Z) (std::string)(X), (std::string)(Y), (std::string)(Z)
#define operValueReturn(X) (std::shared_ptr<operValue>)(X)
#define operValueException(X,Y) (std::string)(X), (std::string)(Y), std::vector<std::tuple<uintmax_t, uintmax_t, std::string>>{}


