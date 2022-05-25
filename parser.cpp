#include "parser.h"
#include <iostream>
#include <sstream>
#include <map>

template <typename T> std::string tostr(const T& t) { 
   std::ostringstream os; 
   os<<t; 
   return os.str(); 
} 

std::shared_ptr<ASTNode> parseAST(std::vector<struct lexerToken> tokens, std::string file)
{
    std::shared_ptr<ASTNode> root = std::make_shared<ASTNode>(ASTNodeBlock(nullptr, 0, 0, file));
    std::shared_ptr<ASTNode> current = std::make_shared<ASTNode>(ASTNodeStatement(root, 0, 0, file));
    root->statements.push_back(current);
    for(size_t i = 0; i < tokens.size(); i++)
    {
        if(current->line == -1 && tokens[i].type != "NEWLINE")
        {
            current->line = tokens[i].line;
            current->column = tokens[i].column;
        }
        if(tokens[i].type == "INTEGER_LITERAL")
            current->statements.push_back(
                std::make_shared<ASTNode> (
                    ASTNodeInt(
                        BigInteger(tokens[i].value), 
                        current, tokens[i].line, 
                        tokens[i].column,
                        file
                    )
                )
            );
        else if(tokens[i].type == "FLOAT_LITERAL")
            current->statements.push_back(
                std::make_shared<ASTNode> (
                    ASTNodeFloat(
                        stof(tokens[i].value), 
                        current, 
                        tokens[i].line, 
                        tokens[i].column,
                        file
                    )
                )
            );
        else if(tokens[i].type == "STRING_LITERAL")
            current->statements.push_back(
                std::make_shared<ASTNode> (
                    ASTNodeString(
                        tokens[i].value, 
                        current, 
                        tokens[i].line, 
                        tokens[i].column,
                        file
                    )
                )
            );
        else if(tokens[i].type == "TOKEN")
            current->statements.push_back(
                std::make_shared<ASTNode> (
                    ASTNodeToken(
                        tokens[i].value, 
                        current, 
                        tokens[i].line, 
                        tokens[i].column,
                        file
                    )
                )
            );
        else if(tokens[i].type == "NEWLINE")
        {
            if(current->parent->type != ASTNodeType::Block)
            {
                std::cout << "Unmatched paren at " << current->line << ":" << current->column << "\n";
                return nullptr;
            }
            if(current->statements.size() > 0 && current->parent->type == ASTNodeType::Block)
            {
                while(current->statements.size() > 3)
                {
                    std::shared_ptr<ASTNode> subStatement = std::make_shared<ASTNode> (
                        ASTNodeStatement(
                            current,
                            current->statements[current->statements.size() - 3]->line, 
                            current->statements[current->statements.size() - 3]->column,
                            file
                        )
                    );
                    subStatement->statements.push_back(current->statements[current->statements.size() - 3]);
                    subStatement->statements.push_back(current->statements[current->statements.size() - 2]);
                    subStatement->statements.push_back(current->statements[current->statements.size() - 1]);
                    current->statements.erase(current->statements.end() - 3, current->statements.end());
                    current->statements.push_back(subStatement);
                }
            
                current->parent->statements.push_back(
                    std::make_shared<ASTNode> (
                        ASTNodeStatement(
                            current->parent, 
                            -1, 
                            -1,
                            file
                        )
                    )
                );
                current = current->parent->statements.back();
            }
        }
        else if(tokens[i].type == "OPEN_PAREN")
        {
            current->statements.push_back(
                std::make_shared<ASTNode> (
                    ASTNodeStatement(
                        current, 
                        tokens[i].line, 
                        tokens[i].column,
                        file
                    )
                )
            );
            current = current->statements.back();
        }
        else if(tokens[i].type == "OPEN_BRACE")
        {
            current->statements.push_back(
                std::make_shared<ASTNode> (
                    ASTNodeBlock(
                        current, 
                        tokens[i].line, 
                        tokens[i].column,
                        file
                    )
                )
            );
            current = current->statements.back();
            
            current->statements.push_back(
                std::make_shared<ASTNode> (
                    ASTNodeStatement(
                        current,
                        -1,    
                        -1,
                        file
                    )
                )
            );
            
            current = current->statements.back();
        }
        else if(tokens[i].type == "CLOSE_PAREN")
        {
            if(current->parent->type != ASTNodeType::Statement)
            {
                std::cout << "Unmatched brace at " << current->line << ":" << current->column << "\n";
                return nullptr;
            }
            while(current->statements.size() > 3)
            {
                std::shared_ptr<ASTNode> subStatement = std::make_shared<ASTNode> (
                    ASTNodeStatement(
                        current,
                        current->statements[current->statements.size() - 3]->line, 
                        current->statements[current->statements.size() - 3]->column,
                        file
                    )
                );
                subStatement->statements.push_back(current->statements[current->statements.size() - 3]);
                subStatement->statements.push_back(current->statements[current->statements.size() - 2]);
                subStatement->statements.push_back(current->statements[current->statements.size() - 1]);
                current->statements.erase(current->statements.end() - 3, current->statements.end());
                current->statements.push_back(subStatement);
            }
            
            current = current->parent;
        }
        else if(tokens[i].type == "CLOSE_BRACE")
        {
            if(current->parent->type != ASTNodeType::Block || current->parent == root)
            {
                std::cout << "Unmatched paren at " << current->line << ":" << current->column << "\n";
                return nullptr;
            }
            if(current->statements.size() == 0)
            {
                current = current->parent;
                current->statements.pop_back();
                current = current->parent;
                continue;
            }
            while(current->statements.size() > 3)
            {
                std::shared_ptr<ASTNode> subStatement = std::make_shared<ASTNode> (
                    ASTNodeStatement(
                        current,
                        current->statements[current->statements.size() - 3]->line, 
                        current->statements[current->statements.size() - 3]->column,
                        file
                    )
                );
                subStatement->statements.push_back(current->statements[current->statements.size() - 3]);
                subStatement->statements.push_back(current->statements[current->statements.size() - 2]);
                subStatement->statements.push_back(current->statements[current->statements.size() - 1]);
                current->statements.erase(current->statements.end() - 3, current->statements.end());
                current->statements.push_back(subStatement);
            }
            current = current->parent->parent;
        }
    }
    
    if(current->statements.size() == 0)
    {
        current = current->parent;
        current->statements.pop_back();
    }
    
    if(current != root)
    {
        if(current->type == ASTNodeType::Block)std::cout << "Unmatched brace at " << current->line << ":" << current->column << "\n";
        else std::cout << "Unmatched paren at " << current->line << ":" << current->column << "\n";
        return nullptr;
    }
    
    return root;
}

std::ostream& operator << (std::ostream& os, ASTNode& node)
{
    return(os << node.toString());
}

std::string ASTNode::toString(int indent)
{
    std::string result(indent, ' ');
    result += getType() + " " + tostr<intmax_t>(line) + ":" + tostr<intmax_t>(column) + " ";
    if(type == ASTNodeType::StringLiteral)result += "'" + string_value + "'";
    else if(type == ASTNodeType::IntegerLiteral)result += (std::string)BigInteger(int_value);
    else if(type == ASTNodeType::FloatLiteral)result += tostr<double>(float_value);
    else if(type == ASTNodeType::IntegerLiteral)result += (std::string)BigInteger(int_value);
    else if(type == ASTNodeType::Token)result += "'" + name + "'";
    else if(type == ASTNodeType::Statement || type == ASTNodeType::Block)
    {
        result += "[\n";
        for(std::shared_ptr<ASTNode>& statement : statements)
        {
            result += statement->toString(indent + 1);
        }
        result += std::string(indent, ' ') + "]";
    }
    else
    {
        throw "Unknown type '" + getType() + "'";
    }
    result += "\n";
    return result;    
}

ASTNode::~ASTNode()
{
    if(type == ASTNodeType::StringLiteral)std::destroy_at(&string_value);
    if(type == ASTNodeType::Token)std::destroy_at(&name);
    if(type == ASTNodeType::Statement || type == ASTNodeType::Block)std::destroy_at(&statements);
}

static const std::map<ASTNodeType, std::string> typeNames = {
{ASTNodeType::IntegerLiteral, "IntegerLiteral"},
{ASTNodeType::FloatLiteral, "FloatLiteral"},
{ASTNodeType::StringLiteral, "StringLiteral"},
{ASTNodeType::Token, "Token"},
{ASTNodeType::Statement, "Statement"},
{ASTNodeType::Block, "Block"}
};

std::string ASTNode::getType()
{
    return typeNames.at(type);
}



