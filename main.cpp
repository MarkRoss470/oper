#include "lexer.h"
#include "parser.h"
#include "runtime.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <memory>

namespace fs = std::filesystem;


int main(int argc, char** argv)
{    
    if(argc == 1)
    {
        //TODO: interpreter from stdin
        std::cout << "Interactive interpreter not yet implemented\n";
        return 1;
    }
    fs::path scriptPath(argv[1]);
    std::string scriptCode;
    try
    {
        if(!fs::exists(scriptPath))
        {
            std::cout << "File not found: '" << argv[1] << "'\n";
            return 1;
        }
        std::ifstream scriptFile(argv[1]);
        std::stringstream scriptCodeStream;
        scriptCodeStream << scriptFile.rdbuf();
        scriptCode = scriptCodeStream.str();
    }
    catch(fs::filesystem_error const&)
    {
        std::cout << "Error opening file '" << argv[1] << "'\n";
        return 1;
    }
    
    std::shared_ptr<std::vector<struct lexerToken>> tokens = tokenise(scriptCode);
    if(tokens == nullptr)return 1;
    
    std::shared_ptr<ASTNode> scriptAST = parseAST(tokens, argv[1]);
    //std::cout << *scriptAST;
    
    if(scriptAST == nullptr)return 1;

    return executeScript(scriptAST);
    
    return 0;
}



