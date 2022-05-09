#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

struct lexerToken
{
    std::string type;
    std::string value;
    intmax_t line;
    intmax_t column;
};

void print_tokens(std::vector<struct lexerToken> tokens);
std::shared_ptr<std::vector<struct lexerToken>> tokenise(std::string script);
