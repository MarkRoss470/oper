#pragma once
#include "parser.h"
#include "operValue.h"

class stackFrame
{
    public:
        bool isStatement;
        std::shared_ptr<operValue> returnValue;
        ~stackFrame();
        union
        {
            struct
            {
                std::shared_ptr<ASTNode> node;
                std::shared_ptr<operValue> values[3];
                std::shared_ptr<operValue> scope;
            } statement;
            struct
            {
                std::shared_ptr<operValue> value;
                int statementIndex;
            } block;
        };
};

#define stackFrameBlock(X,Y) stackFrame{.isStatement=false, .block={.value=X, .statementIndex=Y}}
#define stackFrameStatement(X,Z) stackFrame{.isStatement=true, .statement={.node=X, .values={nullptr, nullptr, nullptr}, .scope=Z}}

int executeScript(std::shared_ptr<ASTNode> script);

