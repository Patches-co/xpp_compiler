#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    // Tokens Especiais
    UNDEF = 0,   // 0
    END_OF_FILE, // 1

    // Tokens Identificadores
    ID,             // 2
    INTEGER_LITERAL // 3
    STRING_LITERAL, // 4

    // Tokens de Categoria
    OP,     // 5
    SEP,    // 6

    // Palavras Reservadas
    CLASS,      // 7
    EXTENDS,    // 8
    INT,        // 9
    STRING,     // 10
    BREAK,      // 11
    PRINT,      // 12
    READ,       // 13
    RETURN,     // 14
    SUPER,      // 15
    IF,         // 16
    ELSE,       // 17
    FOR,        // 18
    NEW,        // 19
    CONSTRUCTOR // 20
};

enum Attributes {
    UNDEF = 0,
 
    // Operadores
    ATTR_OP_LT,     // <
    ATTR_OP_GT,     // >
    ATTR_OP_LE,     // <=
    ATTR_OP_GE,     // >=
    ATTR_OP_PLUS,   // +
    ATTR_OP_MINUS,  // -
    ATTR_OP_MULT,   // *
    ATTR_OP_DIV,    // /
    ATTR_OP_MOD,    // %
    ATTR_OP_ATTR,   // =
    ATTR_OP_EQ,     // ==
    ATTR_OP_NE,     // !=

    // Separadores
    ATTR_SEP_LPAREN,     // (
    ATTR_SEP_RPAREN,     // )
    ATTR_SEP_LBRACKET,   // [
    ATTR_SEP_RBRACKET,   // ]
    ATTR_SEP_LBRACE,     // {
    ATTR_SEP_RBRACE,     // }
    ATTR_SEP_COMMA,      // ,
    ATTR_SEP_DOT,        // .
    ATTR_SEP_SEMICOLON   // ;
}

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
        int line;
    
        Token(int name, int line)
        {
            this->name = name;
            this->attribute = UNDEF;
            this->lexeme = "";
            this->line = line;
        }
        
        Token(int name, int attr, int line)
        {
            this->name = name;
            this->attribute = attr;
            this->lexeme = "";
            this->line = line;
        }

        Token(int name, string l, int line)
        {
            this->name = name;
            this->attribute = UNDEF;
            this->lexeme = l;
            this->line = line;
        }
};