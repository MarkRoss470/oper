#include <iostream>
#include <vector>
#include <cctype>
#include "lexer.h"

void print_tokens(std::vector<struct lexerToken> tokens)
{
    for(size_t i = 0; i < tokens.size(); i++)
    {
        std::cout << "{type=" << tokens[i].type 
        << ", value='" << ((tokens[i].value == "\n") ? "\\n" : tokens[i].value)
        << "', line=" << tokens[i].line 
        << ", column=" << tokens[i].column << "}\n";
    }
}

std::shared_ptr<std::vector<struct lexerToken>> tokenise(std::string script)
{
    std::shared_ptr<std::vector<struct lexerToken>> tokens (new std::vector<struct lexerToken>());
    if(script[script.length() - 1] != '\n')script += '\n';
    intmax_t line = 1;
    intmax_t lineStart = -1;
    for(intmax_t i = 0; i < (intmax_t)script.length(); i++)
    {
        if(script[i] == '\n')
        {
            tokens->push_back(lexerToken {"NEWLINE", "\n", line, i - lineStart});
            line++;
            lineStart = i;
            continue;
        }
        //other than newline, whitespace is not significant
        if(isspace(script[i]))
        {
            continue;
        }
        //comments
        if(script[i] == '#')
        {
            while(script[i+1] != '\n')i++;
            continue;
        }
        //number literal
        if(isdigit(script[i]) || (script[i] == '-' && isdigit(script[i+1])))
        {
            intmax_t numberStart = i;
            bool isFloat = false;
            while(isdigit(script[i+1]))i++;
            if(script[i+1] == '.' && isdigit(script[i+2]))
            {
                isFloat = true;
                i++;
                while(isdigit(script[i+1]))i++;
            }
            if(script[i+1] != '(' && script[i+1] != ')' && script[i+1] != '{' && script[i+1] != '}' && !isspace(script[i+1]))
            {
                while(!isspace(script[++i]));
                std::cout << "invalid token '" << script.substr(numberStart, i-numberStart) << "'\n";
                return nullptr;
            }
            tokens->push_back(lexerToken {isFloat ? "FLOAT_LITERAL" : "INTEGER_LITERAL", script.substr(numberStart, i-numberStart+1), line, numberStart - lineStart});
            continue;
        }
        //string literal
        if(script[i] == '"')
        {
            intmax_t stringStart = i;
            std::string stringContent = "";
            while(1)
            {
                i++;
                if(script[i] == '\n')
                {
                    std::cout << "Unmatched quote at " << line << ":" << stringStart - lineStart << "\n";
                    return nullptr;
                }
                if(script[i] == '"')break;
                if(script[i] == '\\')
                {
                    i++;
                    switch(script[i])
                    {
                        case('n'):stringContent += '\n';break;
                        case('t'):stringContent += '\t';break;
                        case('r'):stringContent += '\r';break;
                        case('"'):stringContent += '"';break;
                        default:stringContent += '\\';i--;break;
                    }
                    continue;
                }
                stringContent += script[i];
            }
            tokens->push_back(lexerToken {"STRING_LITERAL", stringContent, line, stringStart - lineStart});
            continue;
        }
        
        if(script[i] == '('){tokens->push_back(lexerToken{"OPEN_PAREN", "(", line, i - lineStart});continue;}
        if(script[i] == ')'){tokens->push_back(lexerToken{"CLOSE_PAREN", ")", line, i - lineStart});continue;}
        if(script[i] == '{'){tokens->push_back(lexerToken{"OPEN_BRACE", "{", line, i - lineStart});continue;}
        if(script[i] == '}'){tokens->push_back(lexerToken{"CLOSE_BRACE", "}", line, i - lineStart});continue;}
        
        //any other token
        intmax_t tokenStart = i;
        while(script[i+1] != '(' && script[i+1] != ')' && script[i+1] != '{' && script[i+1] != '}' && !isspace(script[i+1]))i++;
        tokens->push_back(lexerToken {"TOKEN", script.substr(tokenStart, i-tokenStart+1), line, tokenStart - lineStart});
        continue;
        
    }
    
    //print_tokens(*tokens);
    
    return tokens;
}





