#include "runtime.h"
#include <iostream>
#include <vector>
//#include <chrono>
//#include <thread>

std::shared_ptr<operValue> constructReference(std::shared_ptr<ASTNode> node, std::shared_ptr<operValue> scope)
{
    if(node->type == ASTNodeType::Statement && node->statements.size() == 1 && node->statements[0]->type == ASTNodeType::Token)
    {
        return scope->getVariable(node->statements[0]->name);
    }
    
    return std::make_shared<operValue> (operValueReference(node, scope));
}


stackFrame::~stackFrame()
{
    if(isStatement)
    {
        std::destroy_at(&statement.node);
    }
    else
    {
        std::destroy_at(&block.value);
    }
}

std::shared_ptr<operValue> resolveTrivial(std::shared_ptr<ASTNode> node)
{
    if(node->type == ASTNodeType::StringLiteral)return std::make_shared<operValue> (operValueString(node->string_value));
    else if(node->type == ASTNodeType::IntegerLiteral)return std::make_shared<operValue> (operValueInt(node->int_value));
    else if(node->type == ASTNodeType::FloatLiteral)return std::make_shared<operValue> (operValueFloat(node->float_value));
    throw "non trivial type '" + node->getType() + "' given";
}

#define SETDEFAULTOPERATOR(X,Y,Z) scriptValue->block_value.variables[X] = std::make_shared<operValue> (operValueBuiltinOperator(X, Y, Z))

int executeScript(std::shared_ptr<ASTNode> script)
{
    std::shared_ptr<operValue> scriptValue = std::make_shared<operValue>(operValueBlock(script, std::shared_ptr<operValue> (nullptr)));
    
    //arithmetic
    SETDEFAULTOPERATOR("=", "Reference", "Value");
    SETDEFAULTOPERATOR("+", "Value", "Value");
    SETDEFAULTOPERATOR("-", "Value", "Value");
    SETDEFAULTOPERATOR("*", "Value", "Value");
    SETDEFAULTOPERATOR("/", "Value", "Value");
    SETDEFAULTOPERATOR("==", "Value", "Value");
    SETDEFAULTOPERATOR("!=", "Value", "Value");
    SETDEFAULTOPERATOR(">", "Value", "Value");
    SETDEFAULTOPERATOR("<", "Value", "Value");
    SETDEFAULTOPERATOR(">=", "Value", "Value");
    SETDEFAULTOPERATOR("<=", "Value", "Value");
    SETDEFAULTOPERATOR("[]", "Value", "Value");
    
    SETDEFAULTOPERATOR(":", "Value", "Value");
    SETDEFAULTOPERATOR("return", "None", "Value");
    SETDEFAULTOPERATOR("type", "None", "Value");
    SETDEFAULTOPERATOR("len", "None", "Value");
    SETDEFAULTOPERATOR("this", "None", "Value");
    
    //exception handling
    SETDEFAULTOPERATOR("try", "None", "Value");
    SETDEFAULTOPERATOR("catch", "None", "Value");
    SETDEFAULTOPERATOR("throw", "Value", "Value");
    SETDEFAULTOPERATOR("message", "None", "Value");
    SETDEFAULTOPERATOR("extype", "None", "Value");
    
    //type casting
    SETDEFAULTOPERATOR("int", "None", "Value");
    SETDEFAULTOPERATOR("float", "None", "Value");
    SETDEFAULTOPERATOR("string", "None", "Value");
    SETDEFAULTOPERATOR("bool", "None", "Value");
    
    //IO
    SETDEFAULTOPERATOR("print", "None", "Value");
    SETDEFAULTOPERATOR("input", "None", "Value");
    
    //consts
    scriptValue->block_value.variables["True"] = std::make_shared<operValue> (operValueBool(true));
    scriptValue->block_value.variables["False"] = std::make_shared<operValue> (operValueBool(false));
    scriptValue->block_value.variables["None"] = std::make_shared<operValue> (operValueNone());
    
    std::vector<std::shared_ptr<stackFrame>> callStack;
    callStack.push_back(
        std::make_shared<stackFrame> (
            stackFrameBlock(scriptValue, 0)
        )
    );
    
    #define current callStack.back()
    //std::cout << "Entering main loop\n";

    while(callStack.size() > 0)
    {
        //std::cout << "\n";
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        if(current->isStatement)
        {
            if(current->statement.node->statements.size() == 0)
            {
                callStack[callStack.size() - 2]->returnValue = std::make_shared<operValue> (operValueNone());
                callStack.pop_back();
                continue;
            }
            if(current->statement.values[0] && current->statement.values[1] && (current->statement.values[2] || current->statement.node->statements.size() == 2))
            {
                if(current->returnValue != nullptr)
                {
                    callStack[callStack.size() - 2]->returnValue = current->returnValue;
                    callStack.pop_back();
                    continue;
                }
            
                if(current->statement.node->statements.size() == 2)
                {
                    current->statement.values[2] = current->statement.values[1];
                    current->statement.values[1] = current->statement.values[0];
                    current->statement.values[0] = std::make_shared<operValue> (operValueNone());
                }
                
                /*
                std::cout << "Calling '" 
                << current->statement.values[1]->toString() 
                << "' with arguments '" << current->statement.values[0]->toString() << "' and '" 
                << current->statement.values[2]->toString() << "'\n";
                */
                
                if(current->statement.values[1]->type == operValueType::Operator)
                {
                    current->statement.values[1]->operator_value.block->block_value.variables["arg0"] = current->statement.values[0];
                    current->statement.values[1]->operator_value.block->block_value.variables["arg1"] = current->statement.values[2];
                    callStack.push_back(std::make_shared<stackFrame> (stackFrameBlock(
                        std::make_shared<operValue> (operValueBlockCopy(current->statement.values[1]->operator_value.block)),
                        0
                    )));
                }
                else
                {
                    callStack[callStack.size() - 2]->returnValue = current->statement.values[1]->evalOperator(current->statement.values[0], current->statement.values[2], current->statement.scope);
                    callStack.pop_back();
                }
                continue;
            
            }
            else if(current->statement.values[0] && current->statement.node->statements.size() == 1)
            {
                if(current->statement.values[0]->type == operValueType::Reference)
                {
                    if(current->statement.values[0]->reference_value.node->type == ASTNodeType::Statement)
                    {
                        current = std::make_shared<stackFrame> (stackFrameStatement(
                            current->statement.values[0]->reference_value.node,
                            current->statement.values[0]->reference_value.scope
                        ));
                        continue;
                    }
                    else if(current->statement.values[0]->reference_value.node->type == ASTNodeType::Token)
                    {
                        //std::cout << *current->statement.values[0]->reference_value.node->name << "\n";
                        callStack[callStack.size() - 2]->returnValue = current->statement.scope->getVariable(
                            current->statement.values[0]->reference_value.node->name
                        );
                        callStack.pop_back();
                        continue;
                    }
                    if(current->statement.values[0]->reference_value.node->type == ASTNodeType::Statement)
                    {
                        callStack[callStack.size() - 2]->returnValue = std::make_shared<operValue> (operValueBlock(
                            current->statement.values[0]->reference_value.node,
                            current->statement.values[0]->reference_value.scope
                        ));
                        callStack.pop_back();
                        continue;
                    }
                    else
                    {
                        //std::cout << "Trivially resolving " << current->statement.values[0]->reference_value.node->toString() << "\n";
                        callStack[callStack.size() - 2]->returnValue = resolveTrivial(current->statement.values[0]->reference_value.node);
                        callStack.pop_back();
                        continue;
                    }
                }
                else if(current->statement.values[0]->type == operValueType::Block)
                {
                    current = std::make_shared<stackFrame> (stackFrameBlock(
                        current->statement.values[0],
                        0
                    ));
                    continue;
                }
                callStack[callStack.size() - 2]->returnValue = current->statement.values[0];
                callStack.pop_back();
                continue;
            }
            else
            {
                /*
                std::cout << current->statement.values[0] << " "
                    << current->statement.values[1] << " "
                    << current->statement.values[2] << "\n";
                if(current->statement.values[0])std::cout << current->statement.values[0]->toString() << " ";
                if(current->statement.values[1])std::cout << current->statement.values[1]->toString() << " ";
                if(current->statement.values[2])std::cout << current->statement.values[2]->toString() << " ";
                std::cout << "\n";
                */
                
                int operatorIndex = (current->statement.node->statements.size() != 3) ? 0 : 1;
                int subStatement = operatorIndex;
                if(current->statement.node->statements.size() != 1)
                {
                    if(current->statement.values[subStatement] != nullptr)subStatement = 1 - subStatement;
                    if(current->statement.values[subStatement] != nullptr)subStatement = 2;
                }
                //std::cout << subStatement << " " << operatorIndex << "\n";
                //std::cout << current->statement.node->toString();
                if(current->returnValue != nullptr)
                {
                    //std::cout << "Handling return value\n";
                    if(subStatement == operatorIndex)
                    {
                        //std::cout << "Type checking operator\n";
                        if(current->returnValue->type == operValueType::Operator)
                        {
                            if(current->returnValue->operator_value.arg0type == "None")
                            {
                                if(operatorIndex == 1)
                                {
                                    callStack[callStack.size() - 2]->returnValue = std::make_shared<operValue> (
                                        operValueException(
                                            "ArgumentException", 
                                            "'" + current->returnValue->toString() + "' takes only 1 argument, but 2 were given"
                                        )
                                    );
                                    callStack.pop_back();
                                    continue;
                                }
                            }
                            else
                            {
                                if(operatorIndex == 0)
                                {
                                    callStack[callStack.size() - 2]->returnValue = std::make_shared<operValue> (
                                        operValueException(
                                            "ArgumentException", 
                                            "'" + current->returnValue->toString() + "' takes 2 arguments, but only 1 was given"
                                        )
                                    );
                                    callStack.pop_back();
                                    continue;
                                }
                                if(current->returnValue->operator_value.arg0type == "Reference")
                                {
                                    current->statement.values[0] = constructReference(
                                        current->statement.node->statements[0],
                                        current->statement.scope
                                    );
                                }
                            }
                            
                            if(current->returnValue->operator_value.arg1type == "Reference")
                            {
                                current->statement.values[operatorIndex + 1] = constructReference(
                                    current->statement.node->statements[operatorIndex + 1],
                                    current->statement.scope
                                );
                            }
                        }
                        else if(current->returnValue->type == operValueType::BuiltinOperator)
                        {
                            //std::cout << "Checking arguments for '" << current->returnValue->toString() << "'\n";
                            if(current->returnValue->builtin_operator_value.arg0type == "None")
                            {
                                if(operatorIndex == 1)
                                {
                                    callStack[callStack.size() - 2]->returnValue = std::make_shared<operValue> (
                                        operValueException(
                                            "ArgumentException", 
                                            "'" + current->returnValue->toString() + "' takes only 1 argument, but 2 were given"
                                        )
                                    );
                                    callStack.pop_back();
                                    continue;
                                }
                            }
                            else 
                            {
                                if(operatorIndex == 0)
                                {
                                    callStack[callStack.size() - 2]->returnValue = std::make_shared<operValue> (
                                        operValueException(
                                            "ArgumentException", 
                                            "'" + current->returnValue->toString() + "' takes 2 arguments, but only 1 was given"
                                        )
                                    );
                                    callStack.pop_back();
                                    continue;
                                }
                                if(current->returnValue->builtin_operator_value.arg0type == "Reference")
                                {
                                    current->statement.values[0] = constructReference(
                                        current->statement.node->statements[0],
                                        current->statement.scope
                                    );
                                }
                            }
                            
                            if(current->returnValue->builtin_operator_value.arg1type == "Reference")
                            {
                                current->statement.values[2] = constructReference(
                                    current->statement.node->statements[2],
                                    current->statement.scope
                                );
                            }
                        }
                        else if(current->returnValue->type == operValueType::Exception)
                        {
                            callStack[callStack.size() - 2]->returnValue = current->returnValue;
                            callStack.pop_back();
                            continue;
                        }
                        else if(current->statement.node->statements.size() > 1)
                        {
                            callStack[callStack.size() - 2]->returnValue = std::make_shared<operValue> (
                                operValueException(
                                    "TypeException", 
                                    "'" + current->returnValue->toString() + "' is not an operator"
                                )
                            );
                            callStack.pop_back();
                            continue;
                        }
                    }
                    current->statement.values[subStatement] = current->returnValue;
                    current->returnValue = nullptr;
                    continue;
                }
                
                //std::cout << subStatement << " " << (*current->statement.node->statements).size() << "\n";
                //std::cout << (*current->statement.node->statements)[subStatement]->toString();
                if(current->statement.node->statements[subStatement]->type == ASTNodeType::Statement)
                {
                    callStack.push_back(std::make_shared<stackFrame> (
                        stackFrameStatement(current->statement.node->statements[subStatement], current->statement.scope)
                    ));
                }
                else if(current->statement.node->statements[subStatement]->type == ASTNodeType::Block)
                {
                    current->returnValue = std::make_shared<operValue> (
                        operValueBlock(current->statement.node->statements[subStatement], current->statement.scope)
                    );
                }
                else if(current->statement.node->statements[subStatement]->type == ASTNodeType::Token)
                {
                    current->returnValue = current->statement.scope->getVariable(
                        current->statement.node->statements[subStatement]->name
                    );
                }
                else
                {
                    current->returnValue = resolveTrivial(current->statement.node->statements[subStatement]);
                }
                
            }    
        }
        else
        {
            if(current->returnValue)
            {
                //std::cout << "Return value was " << current->returnValue->toString() << "\n";
                //if previous statement returned a block, execute it
                if(current->returnValue->type == operValueType::Block)
                {
                    callStack.push_back(std::make_shared<stackFrame> (
                        stackFrameBlock(current->returnValue, 0)
                    ));
                    continue;
                }
                //if the previous statement returned a ReturnValue, return it
                if(current->returnValue->type == operValueType::ReturnValue)
                {
                    if(callStack.size() == 1)
                    {
                        callStack[callStack.size() - 2]->returnValue = std::make_shared<operValue> (
                            operValueException(
                                "TypeException",
                                "return outside a block"
                            )
                        );
                        callStack.pop_back();
                        continue;
                    }
                    callStack[callStack.size() - 2]->returnValue = current->returnValue->return_value;
                    callStack.pop_back();
                    continue;
                }
                //propagate exceptions up callstack
                if(current->returnValue->type == operValueType::Exception)
                {
                    
                    current->returnValue->exception_value.trace.push_back(std::make_tuple(
                        current->block.value->block_value.block->statements[current->block.statementIndex - 1]->line,
                        current->block.value->block_value.block->statements[current->block.statementIndex - 1]->column,
                        current->block.value->block_value.block->statements[current->block.statementIndex - 1]->file
                    ));

                    if(callStack.size() == 1)
                    {
                        std::cout << current->returnValue->exception_value.type << ": " 
                        << current->returnValue->exception_value.message << "\n";
                        for(size_t i = 0; i < current->returnValue->exception_value.trace.size(); i++)
                        {
                            std::cout << std::get<0>(current->returnValue->exception_value.trace[i]) << ":"
                                << std::get<1>(current->returnValue->exception_value.trace[i]) << " in '"
                                << std::get<2>(current->returnValue->exception_value.trace[i]) << "'\n";
                        }
                        return 1;
                    }
                    callStack[callStack.size() - 2]->returnValue = current->returnValue;
                    callStack.pop_back();
                    continue;
                }
            
            }
            
            //std::cout << current->block.statementIndex << "\n"
            // << current->block.value->block_value.block->toString() << "\n";
            if(current->block.statementIndex == (int)current->block.value->block_value.block->statements.size())
            {
                if(callStack.size() > 1)
                {
                    callStack[callStack.size() - 2]->returnValue = std::make_shared<operValue> (operValueNone());
                }
                callStack.pop_back();
                continue;
            }
            current->block.statementIndex++;
            
            callStack.push_back(std::make_shared<stackFrame> (
                stackFrameStatement(current->block.value->block_value.block->statements[current->block.statementIndex-1], current->block.value)
            ));
        }
    }
    return 0;
}








