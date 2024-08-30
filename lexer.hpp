#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>



class Token{
    public:
        Token(std::string typeSet, std::string valueSet = "");
        std::string getValue();
        std::string getType();
        void setValue(std::string newValue);
        void setType(std::string newType);
        std::string asString();
        bool operator==(Token other){return type == other.type && value == other.value;}
        bool operator!=(Token other){return !(type == other.type && value == other.value);}
    private:
        std::string type;
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