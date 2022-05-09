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
    if(arg0->type == "Int" && arg1->type == "Int")
        return std::shared_ptr<operValue> (new operValueInt(arg0->int_value + arg1->int_value));
    
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == "Int")arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == "Float")arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == "Int")arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == "Float")arg1float = arg1->float_value;
        else break;
        
        return std::shared_ptr<operValue> (new operValueFloat(
            arg0float + arg1float
        ));
    }
    while(false);
    
    return std::shared_ptr<operValue> (new operValueString(arg0->toString() + arg1->toString()));
}

std::shared_ptr<operValue> operatorSubtract(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type == "Int" && arg1->type == "Int")
        return std::shared_ptr<operValue> (new operValueInt(arg0->int_value - arg1->int_value));
    
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == "Int")arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == "Float")arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == "Int")arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == "Float")arg1float = arg1->float_value;
        else break;
        
        return std::shared_ptr<operValue> (new operValueFloat(
            arg0float - arg1float
        ));
    }
    while(false);
    
    if(arg0->type == "String" && arg1->type == "Int")
    {
        //No risk of overflow here as passing too large an int gives max value instead 
        if(arg0->string_value.size() < fromstr<size_t>((std::string)arg1->int_value))
        {
            return std::shared_ptr<operValue> (new operValueException("SubstringException", "String too short"));
            
        }
        if(arg1->int_value < BigInteger(0))
        {
            return std::shared_ptr<operValue> (new operValueException("SubstringException", "Index cannot be negative"));
        }
        return std::shared_ptr<operValue> (
            new operValueString(arg0->string_value.substr(0, arg0->string_value.size() - fromstr<size_t>((std::string)arg1->int_value))
        ));
    }
    
    if(arg0->type == "Int" && arg1->type == "String")
    {
        //No risk of overflow here as passing too large an int gives max value instead
        if(arg1->string_value.size() < fromstr<size_t>((std::string)arg0->int_value))
        {
            return std::shared_ptr<operValue> (new operValueException("SubstringException", "String too short"));
        }
        if(arg0->int_value < 0)
        {
            return std::shared_ptr<operValue> (new operValueException("SubstringException", "Index cannot be negative"));
        }
        return std::shared_ptr<operValue> (
            new operValueString(arg1->string_value.substr(fromstr<size_t>((std::string)arg0->int_value), arg1->string_value.size() - fromstr<size_t>((std::string)arg0->int_value))
        ));
    }

    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '-'>: '" + arg0->type + "' and '" + arg1->type
        )
    );
}

std::shared_ptr<operValue> operatorMultiply(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type == "Int" && arg1->type == "Int")
        return std::shared_ptr<operValue> (new operValueInt(arg0->int_value * arg1->int_value));
    
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == "Int")arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == "Float")arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == "Int")arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == "Float")arg1float = arg1->float_value;
        else break;
        
        return std::shared_ptr<operValue> (new operValueFloat(
            arg0float * arg1float
        ));
    }
    while(false);
    
    if(arg0->type == "Bool")
    {
        if(arg0->bool_value)return arg1;
        else return std::shared_ptr<operValue> (new operValueNone());
    }
    
    if(arg0->type == "String" && arg1->type == "Int")
    {
        std::string result = "";
        for(BigInteger i = 0; i < fromstr<size_t>((std::string)arg1->int_value); i++)
        {
            result += arg0->string_value;
        }
        return std::shared_ptr<operValue> (new operValueString(result));
    }
    
    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '*'>: '" + arg0->type + "' and '" + arg1->type
        )
    );
}

std::shared_ptr<operValue> operatorDivide(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type == "Int" && arg1->type == "Int")
    {
        if(arg1->int_value == 0)
        {
            return std::shared_ptr<operValue> (
                new operValueException(
                    "ZeroDivisionException", 
                    "Division by zero"
                )
            );
        }
        if(arg0->int_value % arg1->int_value == 0)
        {
            return std::shared_ptr<operValue> (new operValueInt(arg0->int_value / arg1->int_value));
        }
    }
    
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == "Int")arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == "Float")arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == "Int")arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == "Float")arg1float = arg1->float_value;
        else break;
        
        if(arg1float == 0)
        {
            return std::shared_ptr<operValue> (
                new operValueException(
                    "ZeroDivisionException", 
                    "Division by zero"
                )
            );
        }
        
        return std::shared_ptr<operValue> (new operValueFloat(
            arg0float / arg1float
        ));
    }
    while(false);
    
    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '/'>: '" + arg0->type + "' and '" + arg1->type
        )
    );
}

std::shared_ptr<operValue> operatorGreater(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == "Int")arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == "Float")arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == "Int")arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == "Float")arg1float = arg1->float_value;
        else break;
        
        return std::shared_ptr<operValue> (new operValueBool(
            arg0float > arg1float
        ));
    }
    while(false);
    
    if(arg0->type == "String" && arg1->type == "String")
    {
        return std::shared_ptr<operValue> (new operValueBool(
            arg0->string_value.compare(arg1->string_value) > 0
        ));
    }
    
    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '>'>: '" + arg0->type + "' and '" + arg1->type
        )
    );
}

std::shared_ptr<operValue> operatorLesser(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == "Int")arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == "Float")arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == "Int")arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == "Float")arg1float = arg1->float_value;
        else break;
        
        return std::shared_ptr<operValue> (new operValueBool(
            arg0float < arg1float
        ));
    }
    while(false);
    
    if(arg0->type == "String" && arg1->type == "String")
    {
        return std::shared_ptr<operValue> (new operValueBool(
            arg0->string_value.compare(arg1->string_value) < 0
        ));
    }
    
    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '<'>: '" + arg0->type + "' and '" + arg1->type
        )
    );
}

std::shared_ptr<operValue> operatorGreaterEqual(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == "Int")arg0float = (std::stod((std::string)arg0->int_value));
        else if(arg0->type == "Float")arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == "Int")arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == "Float")arg1float = arg1->float_value;
        else break;
        
        return std::shared_ptr<operValue> (new operValueBool(
            arg0float >= arg1float
        ));
    }
    while(false);
    
    if(arg0->type == "String" && arg1->type == "String")
    {
        return std::shared_ptr<operValue> (new operValueBool(
            arg0->string_value.compare(arg1->string_value) >= 0
        ));
    }
    
    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '>='>: '" + arg0->type + "' and '" + arg1->type
        )
    );
}

std::shared_ptr<operValue> operatorLesserEqual(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == "Int")arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == "Float")arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == "Int")arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == "Float")arg1float = arg1->float_value;
        else break;
        
        return std::shared_ptr<operValue> (new operValueBool(
            arg0float <= arg1float
        ));
    }
    while(false);
    
    if(arg0->type == "String" && arg1->type == "String")
    {
        return std::shared_ptr<operValue> (new operValueBool(
            arg0->string_value.compare(arg1->string_value) <= 0
        ));
    }
    
    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator '<='>: '" + arg0->type + "' and '" + arg1->type
        )
    );
}

std::shared_ptr<operValue> operatorEquals(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    do
    {
        double arg0float;
        double arg1float;
        if(arg0->type == "Int")arg0float = std::stod((std::string)arg0->int_value);
        else if(arg0->type == "Float")arg0float = arg0->float_value;
        else break;
        
        if(arg1->type == "Int")arg1float = std::stod((std::string)arg1->int_value);
        else if(arg1->type == "Float")arg1float = arg1->float_value;
        else break;
        
        return std::shared_ptr<operValue> (new operValueBool(
            arg0float == arg1float
        ));
    }
    while(false);
    if(arg0->type != arg1->type)return std::shared_ptr<operValue> (new operValueBool(false));
    //None, Int, Float, Bool, String are compared by value
    if(arg0->type == "None")return std::shared_ptr<operValue> (new operValueBool(true));
    if(arg0->type == "Bool")return std::shared_ptr<operValue> (new operValueBool(arg0->bool_value == arg1->bool_value));
    if(arg0->type == "String")return std::shared_ptr<operValue> (new operValueBool(arg0->string_value == arg1->string_value));
    //Anything else compared by reference
    return std::shared_ptr<operValue> (new operValueBool(arg0.get() == arg1.get()));
}

std::shared_ptr<operValue> createOperator(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type == "Block" && arg1->type == "String")
    {
        if(arg1->string_value != "Value" && arg1->string_value != "Reference")
        {
            return std::shared_ptr<operValue> (
                new operValueException(
                    "ArgumentException", 
                    "Invalid parameter type '" + arg1->string_value + "'"
                )
            );
        }
        return std::shared_ptr<operValue> (new operValueOperator(arg0, "None", arg1->string_value));
    }
    if(arg0->type == "Operator" && arg1->type == "String")
    {
        if(arg1->string_value != "Value" && arg1->string_value != "Reference")
        {
             return std::shared_ptr<operValue> (
                new operValueException(
                    "ArgumentException", 
                    "Invalid parameter type '" + arg1->string_value + "'"
                )
            );
        }
        arg0->operator_value.arg1type = arg1->string_value;
        return arg0;
    }
    if(arg0->type == "String" && arg1->type == "Operator")
    {
        if(arg0->string_value != "Value" && arg0->string_value != "Reference" && arg0->string_value != "None")
        {
             return std::shared_ptr<operValue> (
                new operValueException(
                    "ArgumentException", 
                    "Invalid parameter type '" + arg0->string_value + "'"
                )
            );
        }
        arg1->operator_value.arg0type = arg0->string_value;
        return arg1;
    }
    
    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported types for <BuiltinOperator ':'>: '" + arg0->type + "' and '" + arg1->type
        )
    );
}

std::shared_ptr<operValue> operatorLength(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type != "String")
    {
        return std::shared_ptr<operValue> (
            new operValueException(
                "ArgumentException", 
                "Unsupported type for <BuiltinOperator 'len'>: '" + arg1->type + "'"
            )
        );
    }

    return std::shared_ptr<operValue> (new operValueInt((int)arg1->string_value.size()));
}

std::shared_ptr<operValue> operatorThrow(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg0->type != "String" || arg1->type != "String")
    {
        return std::shared_ptr<operValue> (
            new operValueException(
                "ArgumentException", 
                "Unsupported types for <BuiltinOperator 'throw'>: '" + arg0->type + "' and '" + arg1->type
            )
        );
    }

    return std::shared_ptr<operValue> (new operValueException(arg0->string_value, arg1->string_value));
}

std::shared_ptr<operValue> operatorMessage(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type != "Exception")
    {
        return std::shared_ptr<operValue> (
            new operValueException(
                "ArgumentException", 
                "Unsupported type for <BuiltinOperator 'message'>: '" + arg1->type + "'"
            )
        );
    }

    return std::shared_ptr<operValue> (new operValueString(arg1->exception_value.message));
}

std::shared_ptr<operValue> operatorExtype(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type != "Exception")
    {
        return std::shared_ptr<operValue> (
            new operValueException(
                "ArgumentException", 
                "Unsupported type for <BuiltinOperator 'extype'>: '" + arg1->type + "'"
            )
        );
    }

    return std::shared_ptr<operValue> (new operValueString(arg1->exception_value.type));
}

std::shared_ptr<operValue> castString(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    return std::shared_ptr<operValue> (new operValueString(arg1->string_value));
}

std::shared_ptr<operValue> castInt(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type == "Int")return arg1;
    if(arg1->type == "Float")return std::shared_ptr<operValue> (new operValueInt((int)arg1->float_value));
    if(arg1->type == "String")
    {
        try
        {
            return std::shared_ptr<operValue> (new operValueInt(BigInteger(arg1->string_value)));
        }
        catch (std::invalid_argument const& e)
        {
            return std::shared_ptr<operValue> (
                new operValueException(
                    "ArgumentException", 
                    "Invalid integer literal: '" + arg1->string_value + "'"
                )
            );
        }
        catch (std::out_of_range const& e)
        {
             return std::shared_ptr<operValue> (
                new operValueException(
                    "ArgumentException", 
                    "Invalid integer literal: '" + arg1->string_value + "'"
                )
            );
        }
    }
    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported type for <BuiltinOperator 'int'>: '" + arg1->type + "'"
        )
    );
}

std::shared_ptr<operValue> castFloat(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type == "Float")return arg1;
    if(arg1->type == "Int")return std::shared_ptr<operValue> (new operValueFloat(std::stod((std::string)arg1->int_value)));
    if(arg1->type == "String")
    {
        try
        {
            return std::shared_ptr<operValue> (new operValueFloat(std::stod(arg1->string_value)));
        }
        catch (std::invalid_argument const& e)
        {
            return std::shared_ptr<operValue> (
                new operValueException(
                    "ArgumentException", 
                    "Invalid float literal: '" + arg1->string_value + "'"
                )
            );
        }
    }
    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported type for <BuiltinOperator 'float'>: '" + arg1->type + "'"
        )
    );
}

std::shared_ptr<operValue> castBool(std::shared_ptr<operValue> arg0, std::shared_ptr<operValue> arg1)
{
    if(arg1->type == "Bool")return arg1;
    if(arg1->type == "Int")return std::shared_ptr<operValue> (new operValueBool(arg1->int_value != 0));
    if(arg1->type == "String")return std::shared_ptr<operValue> (new operValueBool(arg1->string_value != ""));
    return std::shared_ptr<operValue> (
        new operValueException(
            "ArgumentException", 
            "Unsupported type for <BuiltinOperator 'bool'>: '" + arg1->type + "'"
        )
    );
}

