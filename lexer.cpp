#include "lexer.hpp"
#include <string>

std::string TT_INT = "TT_INT";
std::string TT_PLUS = "TT_PLUS";
std::string TT_MINUS = "TT_MINUS";
std::string TT_TIMES = "TT_TIMES";
std::string TT_DIVIDE = "TT_DIVIDE";
std::string TT_LPAREN = "TT_LPAREN";
std::string TT_RPAREN = "TT_RPAREN";
std::string TT_SEMICOLON = "TT_SEMICOLON";

Token::Token(std::string typeSet, std::string valueSet){
    type = typeSet;
    value = valueSet;
}
std::string Token::getValue(){
    return value;
}
std::string Token::getType(){
    return type;
}
void Token::setValue(std::string newValue){
    value = newValue;
}
void Token::setType(std::string newType){
    type = newType;
}
std::string Token::asString(){
    if (value == "") return type;
    else return type + ": " + value;
}

Lexer::Lexer(std::string input){
    text = input;
}
void Lexer::tokenize(){
    for (pos = 0; pos < (int)text.size(); pos ++){
        char chr = text[pos];
        switch (chr){
            case ' ': case '\t': case '\n':
                break;
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9': case '0':
                tokens.push_back(Token(TT_INT,std::string(1,chr)));
                break;
            case '+':
                tokens.push_back(Token(TT_PLUS));
                break;
            case '-':
                tokens.push_back(Token(TT_MINUS));
                break;
            case '*':
                tokens.push_back(Token(TT_TIMES));
                break;
            case '/':
                tokens.push_back(Token(TT_DIVIDE));
                break;
            case '(':
                tokens.push_back(Token(TT_LPAREN));
                break;
            case ')':
                tokens.push_back(Token(TT_RPAREN));
                break;
            case ';':
                tokens.push_back(Token(TT_SEMICOLON));
                break;
        }
    }
}
std::vector<Token> Lexer::getTokens(){
    return tokens;
}