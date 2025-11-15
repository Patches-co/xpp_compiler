#include "scanner.h"

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;
		SymbolTable* globalST;
    	SymbolTable* currentST;

		void advance();
		void match(int);
		void error(string);
		void initSimbolTable();

		void program();
		void classList();
		void classDecl();
		//CONTINUAR
	public:
		Parser(string);
		void run();
};