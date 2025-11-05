#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
    advance();
}

void
Parser::run()
{
    goal();
}

void
Parser::advance()
{
	lToken = scanner->nextToken();

    //cout << lToken->name << endl;
}

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void 
Parser::goal()
{
	expr();
}

void 
Parser::expr()
{
	term();
	exprLinha();
}

void 
Parser::exprLinha()
{
	if (lToken->name == PLUS)
	{
		advance();
		term();
		exprLinha();
	}
	else if (lToken->name == MINUS)
	{
		advance();
		term();
		exprLinha();
	}
	//else
	//	;
}

void 
Parser::term()
{
	factor();
	termLinha();
}

void 
Parser::termLinha()
{
	if (lToken->name == MULT)
	{
		advance();
		factor();
		termLinha();
	}
	else if (lToken->name == DIV)
	{
		advance();
		factor();
		termLinha();
	}
}

void 
Parser::factor()
{
	if (lToken->name == NUMBER)
		advance();
	else if (lToken->name == ID)
		advance();
	else if (lToken->name == LPAREN)
	{
		advance();
		expr();
		match(RPAREN);
	}
	else
		error("Fator mal formado!");
}

void
Parser::error(string str)
{
	cout << str << endl;

	exit(EXIT_FAILURE);
}