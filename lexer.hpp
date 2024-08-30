#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

enum TokenType{
    INT,PLUS,MINUS,TIMES,DIVIDE,LPAREN,RPAREN,SEMICOLON
};

class Token{
    public:
        Token(TokenType typeSet, std::string valueSet = "");
        std::string getValue();
        TokenType getType();
        void setValue(std::string newValue);
        void setType(TokenType newType);
        std::string asString();
        bool operator==(Token other){return type == other.type && value == other.value;}
        bool operator!=(Token other){return !(type == other.type && value == other.value);}
    private:
        TokenType type;
        std::string value;
};

class Lexer{
    public:
        Lexer(std::string input);
        void tokenize();
        std::vector<Token> getTokens();
    private:
         int pos;
        std::string text;
        std::vector<Token> tokens;
};

#endif