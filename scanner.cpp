#include "scanner.h"    

//Construtor
Scanner::Scanner(string input)
{
    this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;
    pos = 0;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    int state = 0;//estado inicial
    string lexeme;

    while (true)
    {
        switch (state)
        {
            case 0://case 9: case 12: case 22:
                if (input[pos] == '\0')
                {
                    tok = new Token(END_OF_FILE);
                    return tok;
                }
                else if (input[pos] == '<')
                    state = 1;
                else if (input[pos] == '=')
                    state = 5;
                else if (input[pos] == '>')
                    state = 6;
                else if (isalpha(input[pos]))
                {
                    state = 10;
                    lexeme.push_back(input[pos]);
                }
                else if (isdigit(input[pos]))
                {
                    state = 13;
                    lexeme.push_back(input[pos]);
                }
                else if (isspace(input[pos]))
                    state = 23;
                else
                    lexicalError();

                pos++;

                break;

            case 1:
                if (input[pos] == '=')
                    state = 2;
                else if (input[pos] == '>')
                    state = 3;
                else
                    state = 4;

                pos++;

                break;

            case 2:
                tok = new Token(RELOP, LE);
                return tok;

            case 3:
                tok = new Token(RELOP, NE);
                return tok;

            case 4:
                pos--;
                tok = new Token(RELOP, LT);
                return tok;

            case 5:
                tok = new Token(RELOP, EQ);
                return tok;

            case 6:
                if (input[pos] == '=')
                    state = 7;
                else
                    state = 8;

                pos++;

                break;

            case 7:
                tok = new Token(RELOP, GE);
                return tok;

            case 8:
                pos--;
                tok = new Token(RELOP, GT);
                return tok;

            case 10:
                if (!isalnum(input[pos]))
                    state = 11;

                lexeme.push_back(input[pos]);
                pos++;

                break;

            case 11:
                pos--;
                lexeme.pop_back();
                tok = new Token(ID);//Não estamos diferenciando IDs de palavras reservadas ainda
                tok->lexeme = lexeme;
                return tok;

            case 13:
                if (input[pos] == '.')
                    state = 14;
                else if (input[pos] == 'E')
                    state = 16;
                else if (!isdigit(input[pos]))
                    state = 20;

                lexeme.push_back(input[pos]);
                pos++;

                break;

            case 14:
                if (isdigit(input[pos]))
                    state = 15;
                else
                    lexicalError();

                lexeme.push_back(input[pos]);
                pos++;

                break;

            case 15:
                if (input[pos] == 'E')
                    state = 16;
                else if (!isdigit(input[pos]))
                    state = 21;

                lexeme.push_back(input[pos]);
                pos++;

                break;

            case 16:
                if (input[pos] == '+' || input[pos] == '-')
                    state = 17;
                else if (isdigit(input[pos]))
                    state = 18;
                else
                    lexicalError();

                lexeme.push_back(input[pos]);
                pos++;

                break;

            case 17:
                if (isdigit(input[pos]))
                    state = 18;
                else
                    lexicalError();

                lexeme.push_back(input[pos]);
                pos++;

                break;

            case 18:
                if (!isdigit(input[pos]))
                    state = 19;

                lexeme.push_back(input[pos]);
                pos++;

                break;

            case 19:
                pos--;
                lexeme.pop_back();
                tok = new Token(NUMBER, DOUBLE_LITERAL);
                tok->lexeme = lexeme;
                return tok;

            case 20:
                pos--;
                lexeme.pop_back();
                tok = new Token(NUMBER, INTEGER_LITERAL);
                tok->lexeme = lexeme;
                return tok;

            case 21:
                pos--;
                lexeme.pop_back();
                tok = new Token(NUMBER, FLOAT_LITERAL);
                tok->lexeme = lexeme;
                return tok;

            case 23:
                if (!isspace(input[pos]))
                    state = 24;

                pos++;

                break;

            case 24:
                pos--;
                state = 0;

                break;

        }
    }
}//Fim nextToken

void 
Scanner::lexicalError()
{
    cout << "Token mal formado\n";
    
    exit(EXIT_FAILURE);
}
