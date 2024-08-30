#include <vector>
#include <string>

class token{
    public:
        std::string type;
        std::string value;
        token(std::string typeSet, std::string valueSet = ""){
            type = typeSet;
            value = valueSet;
        }
        ~token();//leave se std::vector works
    private:

};

class interpreter{
    public:
        interpreter(std::vector<token> tokenSet){
            tokenList = tokenSet;
        }
    private:
        std::vector<token> tokenList
};