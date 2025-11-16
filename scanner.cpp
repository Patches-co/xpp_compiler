#include "scanner.h"
#include <cctype>
#include <stdexcept>

// Construtor
Scanner::Scanner(string input, SymbolTable* st)
{
    this->input = input;
    this->st = st;
    this->pos = 0;
    this->line = 1;

    if (input.length() > 0) {
        this->currentChar = input[pos];
    } else {
        this->currentChar = '\0';
    }
}

int Scanner::getLine()
{
    return this->line;
}


void Scanner::readChar() // Avança um caractere
{
    pos++; // Avança a posição
    if (pos < input.length()) {
        currentChar = input[pos];
        if (currentChar == '\n') {
            line++; // Incrementa a linha
        }
    } else {
        currentChar = '\0'; // Fim do ficheiro
    }
}

Token* Scanner::nextToken()
{
    while (currentChar != '\0') {
        // Ignorar espaços em branco
        if (isspace(currentChar)) {
            skipWhitespace();
            continue;
        }

        // Ignorar comentários
        if (currentChar == '/') {
            if (pos + 1 < input.length() && (input[pos+1] == '/' || input[pos+1] == '*')) {
                skipComments();
                continue;
            }
        }

        // Reconhecer Tokens
        
        // 1. ID ou Palavra-Chave
        if (isalpha(currentChar) || currentChar == '_') {
            return processIdentifierOrKeyword();
        }

        // 2. INTEGER_LITERAL
        if (isdigit(currentChar)) {
            return processNumber();
        }

        // 5. STRING_LITERAL
        if (currentChar == '"') {
            return processString();
        }

        // 4. Separadores
        if (string("()[]{},;.").find(currentChar) != string::npos) {
            return processSeparator();
        }

        // 3. Operadores
        if (string("<>=!+-*/%").find(currentChar) != string::npos) {
            return processOperator();
        }

        string errorMsg = "Token mal formado: '";
        errorMsg += currentChar;
        errorMsg += "'";
        lexicalError(errorMsg);
    }

    // End of file
    return new Token(END_OF_FILE, line);
}

// 7. Espaços em branco
void Scanner::skipWhitespace() 
{
    while (currentChar != '\0' && isspace(currentChar)) {
        readChar();
    }
}


// 8. Comentários
void Scanner::skipComments() 
{
    readChar(); // Consome o primeiro '/'

    if (currentChar == '/') { // Comentário(//)
        while (currentChar != '\0' && currentChar != '\n') {
            readChar();
        }
    } else if (currentChar == '*') { // Comentário(/*)
        readChar();
        while (currentChar != '\0') {
            if (currentChar == '*') {
                char nextChar = (pos + 1 < input.length()) ? input[pos+1] : '\0';
                if (nextChar == '/') {
                    readChar();
                    readChar();
                    return; // Fim do comentário
                } else {
                    readChar();
                }
            } else {
                readChar();
            }
        }
    }
}

Token* Scanner::processIdentifierOrKeyword() 
{
    string lexeme;
    int startLine = line;
    
    while (currentChar != '\0' && (isalnum(currentChar) || currentChar == '_')) {
        lexeme += currentChar;
        readChar();
    }

    STEntry* entry = st->get(lexeme);
    
    if (entry != nullptr && entry->reserved) {
        // palavra reservada
        return new Token(entry->token->name, entry->token->lexeme, startLine);
    } else {
        // ID
        return new Token(ID, lexeme, startLine);
    }
}

Token* Scanner::processNumber() 
{
    string lexeme;
    int startLine = line;
    while (currentChar != '\0' && isdigit(currentChar)) {
        lexeme += currentChar;
        readChar();
    }
    return new Token(INTEGER_LITERAL, lexeme, startLine);
}

Token* Scanner::processString() 
{
    string lexeme;
    int startLine = line;
    readChar();

    while (currentChar != '\0' && currentChar != '"') {
        if(currentChar == '\n') {
            lexicalError("String mal formada (quebra de linha)");
        }
        lexeme += currentChar;
        readChar();
    }

    if (currentChar == '"') {
        readChar();
    } else {
        lexicalError("String nao fechada");
    }
    return new Token(STRING_LITERAL, lexeme, startLine);
}

Token* Scanner::processSeparator()
{
    int startLine = line;
    char sep = currentChar;
    readChar();

    switch(sep) {
        case '(': return new Token(SEP, ATTR_SEP_LPAREN, startLine);
        case ')': return new Token(SEP, ATTR_SEP_RPAREN, startLine);
        case '[': return new Token(SEP, ATTR_SEP_LBRACKET, startLine);
        case ']': return new Token(SEP, ATTR_SEP_RBRACKET, startLine);
        case '{': return new Token(SEP, ATTR_SEP_LBRACE, startLine);
        case '}': return new Token(SEP, ATTR_SEP_RBRACE, startLine);
        case ',': return new Token(SEP, ATTR_SEP_COMMA, startLine);
        case '.': return new Token(SEP, ATTR_SEP_DOT, startLine);
        case ';': return new Token(SEP, ATTR_SEP_SEMICOLON, startLine);
        default:  lexicalError("Separador desconhecido"); return nullptr;
    }
}

Token* Scanner::processOperator()
{
    int startLine = line;
    char op1 = currentChar;
    char op2 = (pos + 1 < input.length()) ? input[pos+1] : '\0';

    readChar();

    // OP c/ 2 caracteres
    if (op1 == '<' && op2 == '=') {
        readChar(); return new Token(OP, ATTR_OP_LE, startLine);
    }
    if (op1 == '>' && op2 == '=') {
        readChar(); return new Token(OP, ATTR_OP_GE, startLine);
    }
    if (op1 == '=' && op2 == '=') {
        readChar(); return new Token(OP, ATTR_OP_EQ, startLine);
    }
    if (op1 == '!' && op2 == '=') {
        readChar(); return new Token(OP, ATTR_OP_NE, startLine);
    }

    // OP c/ 1 caracter
    switch(op1) {
        case '<': return new Token(OP, ATTR_OP_LT, startLine);
        case '>': return new Token(OP, ATTR_OP_GT, startLine);
        case '+': return new Token(OP, ATTR_OP_PLUS, startLine);
        case '-': return new Token(OP, ATTR_OP_MINUS, startLine);
        case '*': return new Token(OP, ATTR_OP_MULT, startLine);
        case '/': return new Token(OP, ATTR_OP_DIV, startLine);
        case '%': return new Token(OP, ATTR_OP_MOD, startLine);
        case '=': return new Token(OP, ATTR_OP_ATTR, startLine);
        default:  lexicalError("Operador desconhecido"); return nullptr;
    }
}

void Scanner::lexicalError(const string& message) 
{
    cout << "Erro Lexico na Linha " << line << ": " << message << endl;
    exit(EXIT_FAILURE);
}