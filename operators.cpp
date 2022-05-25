#include "operators.h"
#include <iostream>
#include <sstream>

template <typename T> T fromstr(std::string t) { 
   std::stringstream os;
   os << t;
   T result;
   os >> result;
   return result; 
} 

std::shared_ptr<operValue> operatorAdd(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type == operValueType::Int && arg1->type == operValueType::Int)
        return std::make_shared<operValue> (operValueInt(arg0->int_value + arg1->int_value));
    
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == operValueType::Int)arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == operValueType::Float)arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == operValueType::Int)arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == operValueType::Float)arg1float = arg1->float_value;
        else break;
        
        return std::make_shared<operValue> (operValueFloat(
            arg0float + arg1float
        ));
    }
    while(false);
    
    return std::make_shared<operValue> (operValueString(arg0->toString() + arg1->toString()));
}

std::shared_ptr<operValue> operatorSubtract(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type == operValueType::Int && arg1->type == operValueType::Int)
        return std::make_shared<operValue> (operValueInt(arg0->int_value - arg1->int_value));
    
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == operValueType::Int)arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == operValueType::Float)arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == operValueType::Int)arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == operValueType::Float)arg1float = arg1->float_value;
        else break;
        
        return std::make_shared<operValue> (operValueFloat(
            arg0float - arg1float
        ));
    }
    while(false);
    
    if(arg0->type == operValueType::String && arg1->type == operValueType::Int)
    {
        //No risk of overflow here as passing too large an int gives max value instead 
        if(arg0->string_value.size() < fromstr<size_t>((std::string)arg1->int_value))
        {
            return std::make_shared<operValue> (operValueException("SubstringException", "String too short"));
            
        }
        if(arg1->int_value < BigInteger(0))
        {
            return std::make_shared<operValue> (operValueException("SubstringException", "Index cannot be negative"));
        }
        return std::make_shared<operValue> (
            operValueString(
                arg0->string_value.substr(0, arg0->string_value.size() - fromstr<size_t>((std::string)arg1->int_value))
            )
        );
    }
    
    if(arg0->type == operValueType::Int && arg1->type == operValueType::String)
    {
        //No risk of overflow here as passing too large an int gives max value instead
        if(arg1->string_value.size() < fromstr<size_t>((std::string)arg0->int_value))
        {
            return std::make_shared<operValue> (operValueException("SubstringException", "String too short"));
        }
        if(arg0->int_value < 0)
        {
            return std::make_shared<operValue> (operValueException("SubstringException", "Index cannot be negative"));
        }
        return std::make_shared<operValue> (
            operValueString(arg1->string_value.substr(fromstr<size_t>((std::string)arg0->int_value), arg1->string_value.size() - fromstr<size_t>((std::string)arg0->int_value))
        ));
    }

    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '-'>: '" + arg0->getType() + "' and '" + arg1->getType()
        )
    );
}

std::shared_ptr<operValue> operatorMultiply(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type == operValueType::Int && arg1->type == operValueType::Int)
        return std::make_shared<operValue> (operValueInt(arg0->int_value * arg1->int_value));
    
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == operValueType::Int)arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == operValueType::Float)arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == operValueType::Int)arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == operValueType::Float)arg1float = arg1->float_value;
        else break;
        
        return std::make_shared<operValue> (operValueFloat(
            arg0float * arg1float
        ));
    }
    while(false);
    
    if(arg0->type == operValueType::Bool)
    {
        if(arg0->bool_value)return arg1;
        else return std::make_shared<operValue> (operValueNone());
    }
    
    if(arg0->type == operValueType::String && arg1->type == operValueType::Int)
    {
        std::string result = "";
        for(BigInteger i = 0; i < fromstr<size_t>((std::string)arg1->int_value); i++)
        {
            result += arg0->string_value;
        }
        return std::make_shared<operValue> (operValueString(result));
    }
    
    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '*'>: '" + arg0->getType() + "' and '" + arg1->getType()
        )
    );
}

std::shared_ptr<operValue> operatorDivide(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type == operValueType::Int && arg1->type == operValueType::Int)
    {
        if(arg1->int_value == 0)
        {
            return std::make_shared<operValue>(
     operValueException(
                    "ZeroDivisionException", 
                    "Division by zero"
                )
            );
        }
        if(arg0->int_value % arg1->int_value == 0)
        {
            return std::make_shared<operValue> (operValueInt(arg0->int_value / arg1->int_value));
        }
    }
    
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == operValueType::Int)arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == operValueType::Float)arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == operValueType::Int)arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == operValueType::Float)arg1float = arg1->float_value;
        else break;
        
        if(arg1float == 0)
        {
            return std::make_shared<operValue>(
     operValueException(
                    "ZeroDivisionException", 
                    "Division by zero"
                )
            );
        }
        
        return std::make_shared<operValue> (operValueFloat(
            arg0float / arg1float
        ));
    }
    while(false);
    
    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '/'>: '" + arg0->getType() + "' and '" + arg1->getType()
        )
    );
}

std::shared_ptr<operValue> operatorGreater(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == operValueType::Int)arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == operValueType::Float)arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == operValueType::Int)arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == operValueType::Float)arg1float = arg1->float_value;
        else break;
        
        return std::make_shared<operValue> (operValueBool(
            arg0float > arg1float
        ));
    }
    while(false);
    
    if(arg0->type == operValueType::String && arg1->type == operValueType::String)
    {
        return std::make_shared<operValue> (operValueBool(
            arg0->string_value.compare(arg1->string_value) > 0
        ));
    }
    
    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '>'>: '" + arg0->getType() + "' and '" + arg1->getType()
        )
    );
}

std::shared_ptr<operValue> operatorLesser(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == operValueType::Int)arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == operValueType::Float)arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == operValueType::Int)arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == operValueType::Float)arg1float = arg1->float_value;
        else break;
        
        return std::make_shared<operValue> (operValueBool(
            arg0float < arg1float
        ));
    }
    while(false);
    
    if(arg0->type == operValueType::String && arg1->type == operValueType::String)
    {
        return std::make_shared<operValue> (operValueBool(
            arg0->string_value.compare(arg1->string_value) < 0
        ));
    }
    
    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '<'>: '" + arg0->getType() + "' and '" + arg1->getType()
        )
    );
}

std::shared_ptr<operValue> operatorGreaterEqual(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == operValueType::Int)arg0float = (std::stod((std::string)arg0->int_value));
        else if(arg0->type == operValueType::Float)arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == operValueType::Int)arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == operValueType::Float)arg1float = arg1->float_value;
        else break;
        
        return std::make_shared<operValue> (operValueBool(
            arg0float >= arg1float
        ));
    }
    while(false);
    
    if(arg0->type == operValueType::String && arg1->type == operValueType::String)
    {
        return std::make_shared<operValue> (operValueBool(
            arg0->string_value.compare(arg1->string_value) >= 0
        ));
    }
    
    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '>='>: '" + arg0->getType() + "' and '" + arg1->getType()
        )
    );
}

std::shared_ptr<operValue> operatorLesserEqual(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == operValueType::Int)arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == operValueType::Float)arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == operValueType::Int)arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == operValueType::Float)arg1float = arg1->float_value;
        else break;
        
        return std::make_shared<operValue> (operValueBool(
            arg0float <= arg1float
        ));
    }
    while(false);
    
    if(arg0->type == operValueType::String && arg1->type == operValueType::String)
    {
        return std::make_shared<operValue> (operValueBool(
            arg0->string_value.compare(arg1->string_value) <= 0
        ));
    }
    
    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '<='>: '" + arg0->getType() + "' and '" + arg1->getType()
        )
    );
}

std::shared_ptr<operValue> operatorEquals(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == operValueType::Int)arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == operValueType::Float)arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == operValueType::Int)arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == operValueType::Float)arg1float = arg1->float_value;
        else break;
        
        return std::make_shared<operValue> (operValueBool(
            arg0float == arg1float
        ));
    }
    while(false);
    if(arg0->type != arg1->type)return std::make_shared<operValue> (operValueBool(false));
    //None, Int, Float, Bool, String are compared by value
    if(arg0->type == operValueType::None)return std::make_shared<operValue> (operValueBool(true));
    if(arg0->type == operValueType::Bool)return std::make_shared<operValue> (operValueBool(arg0->bool_value == arg1->bool_value));
    if(arg0->type == operValueType::String)return std::make_shared<operValue> (operValueBool(arg0->string_value == arg1->string_value));
    //Anything else compared by reference
    return std::make_shared<operValue> (operValueBool(arg0.get() == arg1.get()));
}

std::shared_ptr<operValue> createOperator(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type == operValueType::Block && arg1->type == operValueType::String)
    {
        if(arg1->string_value != "Value" && arg1->string_value != "Reference")
        {
            return std::make_shared<operValue>(
     operValueException(
                    "ArgumentException", 
                    "Invalid parameter type '" + arg1->string_value + "'"
                )
            );
        }
        return std::make_shared<operValue> (operValueOperator(arg0, "None", arg1->string_value));
    }
    if(arg0->type == operValueType::Operator && arg1->type == operValueType::String)
    {
        if(arg1->string_value != "Value" && arg1->string_value != "Reference")
        {
             return std::make_shared<operValue>(
     operValueException(
                    "ArgumentException", 
                    "Invalid parameter type '" + arg1->string_value + "'"
                )
            );
        }
        arg0->operator_value.arg1type = arg1->string_value;
        return arg0;
    }
    if(arg0->type == operValueType::String && arg1->type == operValueType::Operator)
    {
        if(arg0->string_value != "Value" && arg0->string_value != "Reference" && arg0->string_value != "None")
        {
             return std::make_shared<operValue>(
     operValueException(
                    "ArgumentException", 
                    "Invalid parameter type '" + arg0->string_value + "'"
                )
            );
        }
        arg1->operator_value.arg0type = arg0->string_value;
        return arg1;
    }
    
    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator ':'>: '" + arg0->getType() + "' and '" + arg1->getType()
        )
    );
}

std::shared_ptr<operValue> operatorLength(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type != operValueType::String)
    {
        return std::make_shared<operValue> (
            operValueException(
                "ArgumentException", 
                "Unsupported type for <BuiltinOperator 'len'>: '" + arg1->getType() + "'"
            )
        );
    }

    return std::make_shared<operValue> (operValueInt((int)arg1->string_value.size()));
}

std::shared_ptr<operValue> operatorThrow(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type != operValueType::String || arg1->type != operValueType::String)
    {
        return std::make_shared<operValue> (
            operValueException(
                "ArgumentException", 
                "Unsupported types for <BuiltinOperator 'throw'>: '" + arg0->getType() + "' and '" + arg1->getType()
            )
        );
    }

    return std::make_shared<operValue> (operValueException(arg0->string_value, arg1->string_value));
}

std::shared_ptr<operValue> operatorMessage(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type != operValueType::Exception)
    {
        return std::make_shared<operValue> (
            operValueException(
                "ArgumentException", 
                "Unsupported type for <BuiltinOperator 'message'>: '" + arg1->getType() + "'"
            )
        );
    }

    return std::make_shared<operValue> (operValueString(arg1->exception_value.message));
}

std::shared_ptr<operValue> operatorExtype(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type != operValueType::Exception)
    {
        return std::make_shared<operValue> (
            operValueException(
                "ArgumentException", 
                "Unsupported type for <BuiltinOperator 'extype'>: '" + arg1->getType() + "'"
            )
        );
    }

    return std::make_shared<operValue> (operValueString(arg1->exception_value.type));
}

std::shared_ptr<operValue> operatorStringIndex(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type != operValueType::String || arg1->type != operValueType::Int)
    {
        return std::make_shared<operValue> (
            operValueException(
                "ArgumentException", 
                "Unsupported types for <BuiltinOperator '[]'>: '" + arg0->getType() + "' and '" + arg1->getType()
            )
        );
    }
    if(arg1->int_value < 0)
    {
        return std::make_shared<operValue> (operValueException("SubstringException", "Index cannot be negative"));
    }
    return std::make_shared<operValue> (operValueString(std::string(1, arg0->string_value[fromstr<size_t>((std::string)arg1->int_value)])));
}

std::shared_ptr<operValue> castString(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    return std::make_shared<operValue> (operValueString(arg1->string_value));
}

std::shared_ptr<operValue> castInt(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type == operValueType::Int)return arg1;
    if(arg1->type == operValueType::Float)return std::make_shared<operValue> (operValueInt((int)arg1->float_value));
    if(arg1->type == operValueType::String)
    {
        try
        {
            return std::make_shared<operValue> (operValueInt(BigInteger(arg1->string_value)));
        }
        catch (std::invalid_argument const& e)
        {
            return std::make_shared<operValue>(
     operValueException(
                    "ArgumentException", 
                    "Invalid integer literal: '" + arg1->string_value + "'"
                )
            );
        }
        catch (std::out_of_range const& e)
        {
             return std::make_shared<operValue>(
     operValueException(
                    "ArgumentException", 
                    "Invalid integer literal: '" + arg1->string_value + "'"
                )
            );
        }
    }
    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported type for <BuiltinOperator 'int'>: '" + arg1->getType() + "'"
        )
    );
}

std::shared_ptr<operValue> castFloat(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type == operValueType::Float)return arg1;
    if(arg1->type == operValueType::Int)return std::make_shared<operValue> (operValueFloat(std::stod((std::string)arg1->int_value)));
    if(arg1->type == operValueType::String)
    {
        try
        {
            return std::make_shared<operValue> (operValueFloat(std::stod(arg1->string_value)));
        }
        catch (std::invalid_argument const& e)
        {
            return std::make_shared<operValue>(
     operValueException(
                    "ArgumentException", 
                    "Invalid float literal: '" + arg1->string_value + "'"
                )
            );
        }
    }
    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported type for <BuiltinOperator 'float'>: '" + arg1->getType() + "'"
        )
    );
}

std::shared_ptr<operValue> castBool(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type == operValueType::Bool)return arg1;
    if(arg1->type == operValueType::Int)return std::make_shared<operValue> (operValueBool(arg1->int_value != 0));
    if(arg1->type == operValueType::String)return std::make_shared<operValue> (operValueBool(arg1->string_value != ""));
    return std::make_shared<operValue> (
        operValueException(
            "ArgumentException", 
            "Unsupported type for <BuiltinOperator 'bool'>: '" + arg1->getType() + "'"
        )
    );
}

