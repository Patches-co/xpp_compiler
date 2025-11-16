#include "parser.h"

Parser::Parser(string input)
{
	currentST = globalST = new SymbolTable();
	initSimbolTable();

	scanner = new Scanner(input, globalST);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
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
Parser::run()
{
	advance();	

	program();
	//TESTE DA TABELA DE SÍMBOLOS
	/*
    currentST = new SymbolTable(currentST);
	currentST = new SymbolTable(currentST);
    if (currentST->add(new STEntry(new Token(ID), "bianca")))
		cout << "Adição de bianca deu certo" << endl;
	else
		cout << "Adição de bianca não deu certo" << endl;

	STEntry* obj = currentST->get("bianca");

	if (obj)
		cout << "Encontrei o símbolo " << obj->lexeme << endl;
	else
		cout << "Não encontrei o símbolo buscado" << endl;

	//Fim do escopo
	currentST = currentST->getParent();

	obj = currentST->get("bianca");

	if (obj)
		cout << "Encontrei o símbolo " << obj->lexeme << endl;
	else
		cout << "Não encontrei o símbolo buscado" << endl;*/

	/////////////////////////////

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

	//classBody();
}

//CONTINUAR

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
