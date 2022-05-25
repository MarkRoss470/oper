#pragma once
#include "parser.h"
#include "operValue.h"

class stackFrame
{
    public:
        inline stackFrame(std::shared_ptr<ASTNode> n, std::shared_ptr<operValue> v[3], std::shared_ptr<operValue> s) : isStatement(true), statement{.node=n, .values={v[0], v[1], v[2]}, .scope=s} {}
        inline stackFrame(std::shared_ptr<operValue> v, int i) : isStatement(false), block{.value=v, .statementIndex=i} {}
    
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

#define stackFrameBlock(X,Y) (std::shared_ptr<operValue>)(X), (int)(Y)
#define stackFrameStatement(X,Z) (std::shared_ptr<ASTNode>)(X), (std::shared_ptr<operValue>[3]){nullptr, nullptr, nullptr}, (std::shared_ptr<operValue>)(Z)

int executeScript(std::shared_ptr<ASTNode> script);

