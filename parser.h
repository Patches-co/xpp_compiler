#include "scanner.h"

class Parser
{
    private:
        Scanner* scanner;
        Token* lToken;

        void advance();
        void match(int);

        void goal();
        void expr();
        void exprLinha();
        void term();
        void termLinha();
        void factor();

        void error(string);
        
    public:
        Parser(string);

        void run();
};
