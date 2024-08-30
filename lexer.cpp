#include "lexer.hpp"
#include <string>

Token::Token(TokenType typeSet, std::string valueSet){
    type = typeSet;
    value = valueSet;
}
std::string Token::getValue(){
    return value;
}
TokenType Token::getType(){
    return type;
}
void Token::setValue(std::string newValue){
    value = newValue;
}
void Token::setType(TokenType newType){
    type = newType;
}
std::string Token::asString(){
    if (value == ""){
        switch (type)
        {
            case PLUS:
                return "PLUS";
                break;
            case MINUS:
                return "MINUS";
                break;
            case TIMES:
                return "TIMES";
                break;
            case DIVIDE:
                return "DIVIDE";
                break;
            case LPAREN:
                return "LPAREN";
                break;
            case RPAREN:
                return "RPAREN";
                break;
            case SEMICOLON:
                return "SEMICOLON";
                break;
        }
    } else {
        return "INT: " + value;
    }
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
                tokens.push_back(Token(INT,std::string(1,chr)));
                break;
            case '+':
                tokens.push_back(Token(PLUS));
                break;
            case '-':
                tokens.push_back(Token(MINUS));
                break;
            case '*':
                tokens.push_back(Token(TIMES));
                break;
            case '/':
                tokens.push_back(Token(DIVIDE));
                break;
            case '(':
                tokens.push_back(Token(LPAREN));
                break;
            case ')':
                tokens.push_back(Token(RPAREN));
                break;
            case ';':
                tokens.push_back(Token(SEMICOLON));
                break;
        }
    }
}
std::vector<Token> Lexer::getTokens(){
    return tokens;
}