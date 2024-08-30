//file for testing certain things use `make -s TEST=TRUE`
#include "lexer.hpp"
#include <iostream>

int main(){
    std::vector<Token> tokens;
    Lexer lexer("5 * (4 + 4);");
    lexer.tokenize();
    tokens = lexer.getTokens();
    std::cout << "[";
    for (auto i:tokens){
        std::cout << i.asString() << (i != tokens[tokens.size()-1] ? ", " : "");
    }
    std::cout << "]\n";
}