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

		void classBody();
		void varDeclListOpt();
		void constructDeclListOpt();
		void methodDeclListOpt();
		
		void varDeclList();
		void varDecl();
		void type();
		void varDeclOpt();

		void constructDeclList();
		void constructDecl();
		void methodBody();
		void paramListOpt();
		void statementsOpt();

		void paramList();
		void param();

		void statements();
		void statement();
		void atribStat();
		void printStat();
		void readStat();
		void returnStat();
		void superStat();
		void ifStat();
		void forStat();
		void lValue();

		void lValueComp();
		void expression();
		void allocExpression();
		void numExpression();
		void numExpressionLinha();
		void term();
		void termLinha();
		void unaryExpression();
		void factor();
		void argListOpt();
		void argList();

		void atribStatOpt();
		void expressionOpt();

		void methodDeclList();
		void methodDecl();

	public:
		Parser(string);
		void run();
};