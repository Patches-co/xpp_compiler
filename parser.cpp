// parser.cpp (COMPLETO E CORRIGIDO)
#include "parser.h"

Parser::Parser(string input)
{
	currentST = globalST = new SymbolTable();
	initSimbolTable();

	scanner = new Scanner(input, globalST);
}

void
Parser::advance() // c/lookahead de 3 tokens
{
	lToken = peekToken;
	peekToken = peek2Token;

	if (lToken->name != END_OF_FILE)
	{
		peek2Token = scanner->nextToken();
	}
}

void
Parser::match(int t)
{
	if (lToken->name == t) // Corrigido (sem || attribute)
		advance();
	else
		error("Erro inesperado");
}

void
Parser::run()
{
	lToken = scanner->nextToken();
	peekToken = scanner->nextToken();
	peek2Token = scanner->nextToken();

	program();
	cout << "Compilação encerrada com sucesso!\n";
}

void
Parser::program()
{
	if (lToken->name == CLASS)
		classList();
}

void
Parser::classList()
{
	do
	{
		classDecl();
	}
	while(lToken->name == CLASS);
}

void
Parser::classDecl()
{
	match(CLASS);
	match(ID);

	if (lToken->name == EXTENDS)
	{
		advance();
		match(ID);
	}

	classBody();
}

void
Parser::classBody()
{
	match(SEP, ATTR_SEP_LBRACE);
	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LBRACE) {
		advance();
	} else {
		error("Esperado '{' para iniciar o corpo da classe");
	}

	varDeclListOpt();
	constructDeclListOpt();
	methodDeclListOpt();

	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_RBRACE) {
		advance();
	} else {
		error("Esperado '}' para fechar o corpo da classe");
	}
}

void
Parser::varDeclListOpt()
{
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
	{
		varDeclList(); 
	}
}

void
Parser::varDeclList()
{
	varDecl();

while ( (lToken->name == INT || lToken->name == STRING) &&
	        (peekToken->name == ID) &&
	        (peek2Token->name == SEP && (peek2Token->attribute == ATTR_SEP_SEMICOLON || 
	                                     peek2Token->attribute == ATTR_SEP_COMMA)) )
	{
		varDecl();
	}
	
	while ( (lToken->name == ID && peekToken->name == ID) &&
	        (peek2Token->name == SEP && (peek2Token->attribute == ATTR_SEP_SEMICOLON ||
	                                     peek2Token->attribute == ATTR_SEP_COMMA)) )
	{
		varDecl();
	}
}

void
Parser::varDecl()
{
	type();
	match(ID);
	varDeclOpt();

	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_SEMICOLON) {
		advance();
	} else {
		error("Esperado ';' ao final da declaracao de variavel");
	}
}

void
Parser::type()
{
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID) {
		advance();
	} else {
		error("Esperado um tipo ('int', 'string' ou ID)");
	}
}

void
Parser::varDeclOpt()
{
	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_COMMA) {
		advance();
		match(ID); 
		varDeclOpt();
	}
}

void
Parser::constructDeclListOpt()
{
	if (lToken->name == CONSTRUCTOR)
	{
		constructDeclList();
	}
}

void
Parser::constructDeclList()
{
	do
	{
		constructDecl();
	}
	while(lToken->name == CONSTRUCTOR);
}

void
Parser::constructDecl()
{
	match(CONSTRUCTOR);
	methodBody();
}

void
Parser::methodBody()
{
	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN) {
		advance();
	} else {
		error("Esperado '(' para iniciar o corpo do metodo/construtor");
	}

	paramListOpt();

	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_RPAREN) {
		advance();
	} else {
		error("Esperado ')' para fechar a lista de parametros");
	}

	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LBRACE) {
		advance();
	} else {
		error("Esperado '{' para iniciar o bloco de statements");
	}

	statementsOpt(); 

	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_RBRACE) {
		advance();
	} else {
		error("Esperado '}' para fechar o bloco de statements");
	}
}

void
Parser::paramListOpt()
{
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
	{
		paramList(); 
	}
}

void
Parser::paramList()
{
	param();
	while (lToken->name == SEP && lToken->attribute == ATTR_SEP_COMMA)
	{
		advance();
		param();
	}
}

void
Parser::param()
{
	type();
	match(ID);
}

void
Parser::statementsOpt()
{
	if (lToken->name != SEP || lToken->attribute != ATTR_SEP_RBRACE)
	{
		statements();
	}
}

void
Parser::statements()
{
	statement();
	while (lToken->name != SEP || lToken->attribute != ATTR_SEP_RBRACE)
	{
		statement();
	}
}

void
Parser::statement()
{
	switch (lToken->name)
	{
		case INT:
		case STRING:
			varDeclList();
			break;

		case ID:
			if (peekToken->name == ID)
			{
				varDeclList();
			}
			else if ((peekToken->name == OP && peekToken->attribute == ATTR_OP_ATTR) ||
						(peekToken->name == SEP && peekToken->attribute == ATTR_SEP_LBRACKET) ||
						(peekToken->name == SEP && peekToken->attribute == ATTR_SEP_DOT))
			{
				atribStat();
				match(SEP, ATTR_SEP_SEMICOLON);
			}
			else
			{
				error("Esperado um ID (para declaracao) ou '=', '[', '.' (para atribuicao) apos um ID");
			}
			break;

		case PRINT:
			printStat();
			match(SEP, ATTR_SEP_SEMICOLON);
			break;
		
		case READ:
			readStat();
			match(SEP, ATTR_SEP_SEMICOLON);
			break;

		case RETURN:
			returnStat();
			match(SEP, ATTR_SEP_SEMICOLON);
			break;

		case SUPER:
			superStat();
			match(SEP, ATTR_SEP_SEMICOLON);
			break;

		case IF:
			ifStat();
			break;

		case FOR:
			forStat();
			break;

		case BREAK:
			advance(); 
			match(SEP, ATTR_SEP_SEMICOLON);
			break;

		case SEP:
			if (lToken->attribute == ATTR_SEP_SEMICOLON) {
				advance();
			} 
			else if (lToken->attribute == ATTR_SEP_LBRACE) {
				advance();
				statements(); 
				match(SEP, ATTR_SEP_RBRACE);
			} else {
				error("Statement inesperado iniciado com este separador");
			}
			break;

		default:
			error("Statement inesperado");
	}
}

void Parser::atribStat()
{
	lValue(); 
	match(OP, ATTR_OP_ATTR); 

	if (lToken->name == NEW) {
		allocExpression();
	} else {
		expression();
	}
}

void Parser::returnStat()
{
	match(RETURN); 
	expression();
}

void Parser::superStat()
{
	match(SUPER); 
	match(SEP, ATTR_SEP_LPAREN);
	argListOpt();
	match(SEP, ATTR_SEP_RPAREN);
}

// *** FUNÇÃO ifStat CORRIGIDA ***
void Parser::ifStat()
{
	match(IF); 
	match(SEP, ATTR_SEP_LPAREN);
	expression();
	match(SEP, ATTR_SEP_RPAREN);

	match(SEP, ATTR_SEP_LBRACE);
	
	// CORREÇÃO: Usar statementsOpt para permitir {}
	statementsOpt(); 
	
	match(SEP, ATTR_SEP_RBRACE);

	// Parte 'else' (Opcional)
	if (lToken->name == ELSE)
	{
		advance();
		match(SEP, ATTR_SEP_LBRACE);
		statementsOpt(); // CORREÇÃO: Consistente
		match(SEP, ATTR_SEP_RBRACE);
	}
}

void Parser::forStat()
{
	match(FOR); 
	match(SEP, ATTR_SEP_LPAREN);
	atribStatOpt(); 
	match(SEP, ATTR_SEP_SEMICOLON);
	expressionOpt();
	match(SEP, ATTR_SEP_SEMICOLON);
	atribStatOpt(); 
	match(SEP, ATTR_SEP_RPAREN);
	match(SEP, ATTR_SEP_LBRACE); 
	statementsOpt(); 
	match(SEP, ATTR_SEP_RBRACE);
}

void Parser::atribStatOpt()
{
	if (lToken->name == ID)
	{
		atribStat();
	}
}

void Parser::expressionOpt()
{
	if (lToken->name == ID || lToken->name == INTEGER_LITERAL ||
		lToken->name == STRING_LITERAL || 
		(lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN))
	{
		expression();
	}
}

void
Parser::match(int t, int attr)
{
	if (lToken->name == t && lToken->attribute == attr)
		advance();
	else
		error("Erro inesperado (esperava atributo)");
}

// *** FUNÇÕES printStat e readStat (DUPLICADAS REMOVIDAS) ***
void Parser::readStat()
{
	match(READ);
	lValue();
}

void Parser::printStat()
{
	match(PRINT);
	expression();
}

// === PILHA DE EXPRESSÕES (Regras 32-41) ===

void Parser::lValue()
{
	match(ID);
	lValueComp();
}

void Parser::lValueComp()
{
	while (true) {
		if (lToken->name == SEP && lToken->attribute == ATTR_SEP_DOT) {
			advance();
			match(ID);

			if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN) {
				advance();
				argListOpt();
				match(SEP, ATTR_SEP_RPAREN);
			}

		} else if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LBRACKET) {
			advance(); 
			expression();
			match(SEP, ATTR_SEP_RBRACKET);
		} else {
			return; 
		}
	}
}

void Parser::expression()
{
	numExpression(); 
	if (lToken->name == OP && (
		lToken->attribute == ATTR_OP_LT || lToken->attribute == ATTR_OP_GT ||
		lToken->attribute == ATTR_OP_LE || lToken->attribute == ATTR_OP_GE ||
		lToken->attribute == ATTR_OP_EQ || lToken->attribute == ATTR_OP_NE
	))
	{
		advance();
		numExpression();
	}
}

void Parser::numExpression()
{
	term();
	numExpressionLinha();
}

void Parser::numExpressionLinha()
{
	if (lToken->name == OP && lToken->attribute == ATTR_OP_PLUS) {
		advance();
		term();
		numExpressionLinha();
	} else if (lToken->name == OP && lToken->attribute == ATTR_OP_MINUS) {
		advance();
		term();
		numExpressionLinha();
	}
}

void Parser::term()
{
	unaryExpression();
	termLinha();
}

void Parser::termLinha()
{
	if (lToken->name == OP && lToken->attribute == ATTR_OP_MULT) {
		advance();
		unaryExpression();
		termLinha();
	} else if (lToken->name == OP && lToken->attribute == ATTR_OP_DIV) {
		advance();
		unaryExpression();
		termLinha();
	} else if (lToken->name == OP && lToken->attribute == ATTR_OP_MOD) {
		advance();
		unaryExpression();
		termLinha();
	}
}

void Parser::unaryExpression()
{
	if (lToken->name == OP && lToken->attribute == ATTR_OP_PLUS) {
		advance();
	} else if (lToken->name == OP && lToken->attribute == ATTR_OP_MINUS) {
		advance();
	}
	factor();
}

void Parser::factor()
{
	if (lToken->name == INTEGER_LITERAL) {
		advance();
	} else if (lToken->name == STRING_LITERAL) {
		advance();
	} else if (lToken->name == ID) {
		lValue();
	} else if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN) {
		advance(); 
		expression();
		match(SEP, ATTR_SEP_RPAREN); 
	} else {
		error("Esperado um Fator (numero, string, LValue ou '( Expression )')");
	}
}

void Parser::argListOpt()
{
	if (lToken->name == ID || lToken->name == INTEGER_LITERAL ||
		lToken->name == STRING_LITERAL || 
		(lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN))
	{
		argList();
	}
}

void Parser::argList()
{
	expression(); 
	while (lToken->name == SEP && lToken->attribute == ATTR_SEP_COMMA)
	{
		advance();
		expression();
	}
}

void Parser::allocExpression()
{
	match(NEW);
	
	if (lToken->name == ID) 
	{
		advance(); 
		if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN) {
			advance(); 
			argListOpt();
			match(SEP, ATTR_SEP_RPAREN);
		} 
		else if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LBRACKET) {
			advance();
			expression();
			match(SEP, ATTR_SEP_RBRACKET); 
		} else {
			error("Esperado '(' ou '[' apos 'new ID' na alocacao");
		}
	} 
	else if (lToken->name == INT || lToken->name == STRING) 
	{
		advance(); 
		match(SEP, ATTR_SEP_LBRACKET);
		expression(); 
		match(SEP, ATTR_SEP_RBRACKET);
	} 
	else 
	{
		error("Esperado um tipo (ID, 'int' ou 'string') apos 'new'");
	}
}

// *** FUNÇÃO methodDeclListOpt (DUPLICADA REMOVIDA) ***
void
Parser::methodDeclListOpt()
{
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
	{
		methodDeclList();
	}
}

void
Parser::methodDeclList()
{
	do
	{
		methodDecl();
	}
	while(lToken->name == INT || lToken->name == STRING || lToken->name == ID);
}

void
Parser::methodDecl()
{
	type();
	match(ID); 
	methodBody(); 
}

void
Parser::initSimbolTable()
{
    Token* t;
    int line = 0;

    t = new Token(CLASS, "class", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(EXTENDS, "extends", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(INT, "int", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(STRING, "string", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(BREAK, "break", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(PRINT, "print", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(READ, "read", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(RETURN, "return", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(SUPER, "super", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(IF, "if", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(ELSE, "else", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(FOR, "for", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(NEW, "new", line);
    globalST->add(new STEntry(t, true));
    
    t = new Token(CONSTRUCTOR, "constructor", line);
    globalST->add(new STEntry(t, true));
}

void
Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;
	exit(EXIT_FAILURE);
}