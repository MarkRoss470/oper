#pragma once
#include "lexer.h"
#include "bigint.h"
#include <vector>

class ASTNode
{
    public:
        ~ASTNode();
        std::string toString(int indent = 0);
        std::string type;
        std::string file;
        std::shared_ptr<ASTNode> parent;
        intmax_t line;
        intmax_t column;
        union
        {
            std::string string_value;
            BigInteger int_value;
            double float_value;
            std::string name;
            std::vector<std::shared_ptr<ASTNode>> statements;
        };
};

#define ASTNodeInt(X,Y,L,C,F) ASTNode{.type="IntegerLiteral", .file=F, .parent=Y, .line=L, .column=C, .int_value=X}
#define ASTNodeFloat(X,Y,L,C,F) ASTNode{.type="FloatLiteral", .file=F, .parent=Y, .line=L, .column=C, .float_value=X}
#define ASTNodeString(X,Y,L,C,F) ASTNode{.type="StringLiteral", .file=F, .parent=Y, .line=L, .column=C, .string_value=std::string(X)}
#define ASTNodeToken(X,Y,L,C,F) ASTNode{.type="Token", .file=F, .parent=Y, .line=L, .column=C, .name=std::string(X)}
#define ASTNodeBlock(Y,L,C,F) ASTNode{.type="Block", .file=F, .parent=Y, .line=L, .column=C, .statements=std::vector<std::shared_ptr<ASTNode>>{}}
#define ASTNodeStatement(Y,L,C,F) ASTNode{.type="Statement", .file=F, .parent=Y, .line=L, .column=C, .statements=std::vector<std::shared_ptr<ASTNode>>{}}

std::shared_ptr<ASTNode> parseAST(std::shared_ptr<std::vector<struct lexerToken>>, std::string);

std::ostream& operator << (std::ostream& os, ASTNode& node);

