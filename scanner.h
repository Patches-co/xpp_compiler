#include "symboltable.h"
#include "token.h"
#include <fstream>
#include <string>

using namespace std;

class Scanner 
{
    private: 
        string input;       // Armazena o texto de entrada
        int pos;            // Posição atual
        int line;           // Linha atual    
        SymbolTable* st;    // Ponteiro p/ tabela de simbolos
        char currentChar;   // Caracter atualmente lido
    

        void readChar();
        void skipWhitespace();
        void skipComments();
        Token* processIdentifierOrKeyword();
        Token* processNumber();
        Token* processString();
        Token* processOperator();
        Token* processSeparator();

    public:
        Scanner(string, SymbolTable*); //Construtor

        int getLine();
    
        Token* nextToken();            //Método que retorna o próximo token da entrada 

        void lexicalError(const string&);     //Método para manipular erros
};
