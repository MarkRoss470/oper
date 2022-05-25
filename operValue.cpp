#include "operValue.h"
#include "operators.h"
#include <iostream>
#include <sstream>
#include <map>

template <typename T> std::string tostr(const T& t) { 
   std::ostringstream os; 
   os<<t;
   return os.str(); 
} 

std::string operValue::toString()
{
    if(type == operValueType::None)return "None";
    else if(type == operValueType::String)return string_value;
    else if(type == operValueType::Int)return (std::string)int_value;
    else if(type == operValueType::Float)return tostr<float>(float_value);
    else if(type == operValueType::Bool)return (bool_value ? "True" : "False");
    else if(type == operValueType::Block)return "<Block: " + tostr<int>(block_value.block->line) + ":" + tostr<int>(block_value.block->column) + ">";
    //TODO: add more info to these?
    //possibly adress
    else if(type == operValueType::Reference)return "<Reference>";
    else if(type == operValueType::Operator)return "<Operator taking '" + operator_value.arg0type + "' and '" + operator_value.arg1type + "'>";
    
    else if(type == operValueType::BuiltinOperator)return "<BuiltinOperator '" + builtin_operator_value.name + "'>";
    else if(type == operValueType::ReturnValue)return "<ReturnValue: '" + return_value->toString() + "'>";
    
    throw "Unknown type '" + getType() + "'";
}

operValue::~operValue()
{
    //*((int*)nullptr) = 100;
    //std::cout << "Destructing " << toString() << "\n";
    //std::cout << type << "\n";
    if(type == operValueType::None || type == operValueType::Bool || type == operValueType::Int || type == operValueType::Float){}
    else if(type == operValueType::String){
        std::destroy_at(&string_value);
    }
    else if(type == operValueType::Block)
    {
        std::destroy_at(&block_value);
    }
    else if(type == operValueType::Reference)
    {
        std::destroy_at(&reference_value);
    }
    else if(type == operValueType::Operator)
    {
        std::destroy_at(&operator_value);
    }
    else if(type == operValueType::BuiltinOperator)
    {
        std::destroy_at(&builtin_operator_value);
    }
    else if(type == operValueType::ReturnValue)
    {
        std::destroy_at(&return_value);
    }
    else if(type == operValueType::Exception)
    {
        std::destroy_at(&exception_value);
    }
    else{std::cout << "Unknown type '" << getType() << "'\n";}
    
}

std::shared_ptr<operValue> operValue::getVariable(std::string name)
{
    if(type != operValueType::Block)throw "Not a block";
    operValue* current = this;
    while(true)
    {
        if(current->block_value.variables.contains(name))return current->block_value.variables[name];
        if(current->block_value.parent == nullptr)
        {
             return std::make_shared<operValue> (
                operValueException(
                    "NameException", 
                    "Variable not found: '" + name + "'"
                )
            );
        }
        current = current->block_value.parent.get();
    }
}

void operValue::setVariable(std::string name, std::shared_ptr<operValue> value)
{
    if(type != operValueType::Block)throw "Not a block";
    operValue* current = this;
    while(true)
    {
        if(current->block_value.variables.contains(name))
        {
            current->block_value.variables[name] = value;
            return;
        }
        if(current->block_value.parent == nullptr)
        {
            block_value.variables[name] = value;
            return;
        }
        current = current->block_value.parent.get();
    }
}

std::shared_ptr<operValue> operValue::evalOperator(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1, std::shared_ptr<operValue> scope)
{
    if(type != operValueType::BuiltinOperator)throw "Not a builtin operator";
    //if not an operator dealing with them, pass on any exceptions
    if(builtin_operator_value.name != "try" && 
        builtin_operator_value.name != "catch" && 
        builtin_operator_value.name != "message" && 
        builtin_operator_value.name != "extype"
    )
    {
        if(arg0->type == operValueType::Exception)return arg0;
        if(arg1->type == operValueType::Exception)return arg1;
    }
    if(builtin_operator_value.name == "=")
    {
        //std::cout << arg0->toString() << " " << arg1->toString() << "\n";
        if(arg0->reference_value.node->type != ASTNodeType::Token)
        {
             return std::make_shared<operValue> (
                operValueException(
                    "AssignmentException", 
                    "Can only assign to a Token, not '" + arg0->reference_value.node->getType() + "'"
                )
            );
        }
        
        arg0->reference_value.scope->setVariable(arg0->reference_value.node->name, arg1);
        
        return std::make_shared<operValue> (operValueNone());
    }
    else if(builtin_operator_value.name == "print")
    {
        std::cout << arg1->toString() << "\n";
        return std::make_shared<operValue> (operValueNone());
    }
    else if(builtin_operator_value.name == "input")
    {
        std::cout << arg1->toString();
        std::string result;
        getline(std::cin, result);
        return std::make_shared<operValue> (operValueString(result));
    }
    else if(builtin_operator_value.name == "return"){return std::make_shared<operValue> (operValueReturn(arg1));}
    else if(builtin_operator_value.name == "type"){return std::make_shared<operValue> (operValueString(arg1->getType()));}
    else if(builtin_operator_value.name == "len"){return operatorLength(arg0, arg1);}
    else if(builtin_operator_value.name == "this"){return scope;}
    else if(builtin_operator_value.name == "+"){return operatorAdd(arg0, arg1);}
    else if(builtin_operator_value.name == "-"){return operatorSubtract(arg0, arg1);}
    else if(builtin_operator_value.name == "*"){return operatorMultiply(arg0, arg1);}
    else if(builtin_operator_value.name == "/"){return operatorDivide(arg0, arg1);}
    else if(builtin_operator_value.name == "=="){return operatorEquals(arg0, arg1);}
    else if(builtin_operator_value.name == ">"){return operatorGreater(arg0, arg1);}
    else if(builtin_operator_value.name == ">="){return operatorGreaterEqual(arg0, arg1);}
    else if(builtin_operator_value.name == "<"){return operatorLesser(arg0, arg1);}
    else if(builtin_operator_value.name == "<="){return operatorLesserEqual(arg0, arg1);}
    else if(builtin_operator_value.name == "[]"){return operatorStringIndex(arg0, arg1);}
    else if(builtin_operator_value.name == "int"){return castInt(arg0, arg1);}
    else if(builtin_operator_value.name == "float"){return castFloat(arg0, arg1);}
    else if(builtin_operator_value.name == "string"){return castString(arg0, arg1);}
    else if(builtin_operator_value.name == "bool"){return castBool(arg0, arg1);}
    else if(builtin_operator_value.name == "try")
    {
        if(arg1->type == operValueType::Exception)
        {
            scope->setVariable("exception", arg1);
        }
        else
        {
            scope->block_value.variables.erase("exception");
        }
        return std::make_shared<operValue> (operValueNone());
    }
    else if(builtin_operator_value.name == "catch")
    {
        if(scope->block_value.variables.contains("exception"))
        {
            return arg1;
        }
        return std::make_shared<operValue> (operValueNone());
    }
    else if(builtin_operator_value.name == "throw"){return operatorThrow(arg0, arg1);}
    else if(builtin_operator_value.name == "message"){return operatorMessage(arg0, arg1);}
    else if(builtin_operator_value.name == "extype"){return operatorExtype(arg0, arg1);}
    
    else if(builtin_operator_value.name == "!=")
    {
        std::shared_ptr<operValue> result = operatorEquals(arg0, arg1);
        result->bool_value ^= true;
        return result;
    }
    else if(builtin_operator_value.name == ":"){return createOperator(arg0, arg1);}
    
    throw "Operator '" + builtin_operator_value.name + "' not implemented";
}

static const std::map<operValueType, std::string> typeNames = {
{operValueType::Int, "Int"},
{operValueType::Float, "Float"},
{operValueType::String, "String"},
{operValueType::Bool, "Bool"},
{operValueType::None, "None"},
{operValueType::Block, "Block"},
{operValueType::Statement, "Statement"},
{operValueType::Reference, "Reference"},
{operValueType::Operator, "Operator"},
{operValueType::BuiltinOperator, "BuiltinOperator"},
{operValueType::ReturnValue, "ReturnValue"},
{operValueType::Exception, "Exception"},
};

std::string operValue::getType()
{
    return typeNames.at(type);
}



