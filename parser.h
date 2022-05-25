#pragma once
#include "lexer.h"
#include "bigint.h"
#include <vector>

enum class ASTNodeType
{
    IntegerLiteral,
    FloatLiteral,
    StringLiteral,
    Token,
    Statement,
    Block
};

class ASTNode
{
    public:
        //IntegerLiteral
        inline ASTNode(std::string f, std::shared_ptr<ASTNode> p, intmax_t l, intmax_t c, BigInteger i) : type(ASTNodeType::IntegerLiteral), file(f), parent(p), line(l), column(c), int_value(i) {}
        //FloatLiteral
        inline ASTNode(std::string f, std::shared_ptr<ASTNode> p, intmax_t l, intmax_t c, double d) : type(ASTNodeType::FloatLiteral), file(f), parent(p), line(l), column(c), float_value(d) {}
        //StringLiteral or Token
        inline ASTNode(ASTNodeType t, std::string f, std::shared_ptr<ASTNode> p, intmax_t l, intmax_t c, std::string s) : type(t), file(f), parent(p), line(l), column(c), string_value(s) {}
        //Statement or Block
        inline ASTNode(ASTNodeType t, std::string f, std::shared_ptr<ASTNode> p, intmax_t l, intmax_t c, std::vector<std::shared_ptr<ASTNode>> s) : type(t), file(f), parent(p), line(l), column(c), statements(s) {}
        ~ASTNode();
        std::string toString(int indent = 0);
        ASTNodeType type;
        std::string getType();
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

#define ASTNodeInt(X,Y,L,C,F) (std::string)(F), (std::shared_ptr<ASTNode>)(Y), (intmax_t)(L), (intmax_t)(C), (BigInteger)(X)
#define ASTNodeFloat(X,Y,L,C,F) (std::string)(F), (std::shared_ptr<ASTNode>)(Y), (intmax_t)(L), (intmax_t)(C), (double)(X)
#define ASTNodeString(X,Y,L,C,F) ASTNodeType::StringLiteral, (std::string)(F), (std::shared_ptr<ASTNode>)(Y), (intmax_t)(L), (intmax_t)(C), (std::string)(X)
#define ASTNodeToken(X,Y,L,C,F) ASTNodeType::Token, (std::string)(F), (std::shared_ptr<ASTNode>)(Y), (intmax_t)(L), (intmax_t)(C), (std::string)(X)
#define ASTNodeBlock(Y,L,C,F) ASTNodeType::Block, (std::string)(F), (std::shared_ptr<ASTNode>)(Y), (intmax_t)(L), (intmax_t)(C), (std::vector<std::shared_ptr<ASTNode>>){}
#define ASTNodeStatement(Y,L,C,F) ASTNodeType::Statement, (std::string)(F), (std::shared_ptr<ASTNode>)(Y), (intmax_t)(L), (intmax_t)(C), (std::vector<std::shared_ptr<ASTNode>>){}

std::shared_ptr<ASTNode> parseAST(std::vector<struct lexerToken>, std::string);

std::ostream& operator << (std::ostream& os, ASTNode& node);

