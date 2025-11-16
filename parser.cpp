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

	classBody();
}

void
Parser::classBody()
{
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
		varDeclList(); // <-- Próximo passo
	}
	// Se não (ε - vazio), não fazemos nada.
}

void
Parser::varDeclList()
{
	varDecl();
	while (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
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
		advance(); // Consome o token de tipo
	} else {
		error("Esperado um tipo ('int', 'string' ou ID)");
	}
}

void
Parser::varDeclOpt()
{
	// Verifica se a regra começa com ','
	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_COMMA) {
		advance(); // Consome a ','
		match(ID); // Consome o ID
		
		// Chama a si própria recursivamente para
		// tratar múltiplas declarações (ex: int a, b, c;)
		varDeclOpt();
	}
	// Se não (ε - vazio), não faz nada.
}

void
Parser::constructDeclListOpt()
{
	// Como sabemos se um ConstructDeclList está a começar?
	// Olhamos a Regra 12: começa com 'constructor'.
	if (lToken->name == CONSTRUCTOR)
	{
		constructDeclList(); // <-- Próximo passo
	}
	// Se não (ε - vazio), não fazemos nada.
}

void
Parser::constructDeclList()
{
	// O constructDeclListOpt() já garantiu que temos
	// pelo menos um 'constructor'.
	do
	{
		constructDecl();
	}
	// Continua enquanto o próximo token for 'constructor'
	while(lToken->name == CONSTRUCTOR);
}

void
Parser::constructDecl()
{
	match(CONSTRUCTOR); // Consome 'constructor'
	methodBody();       // Chama a regra MethodBody
}

void
Parser::methodBody()
{
	// "match" do token '('
	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN) {
		advance();
	} else {
		error("Esperado '(' para iniciar o corpo do metodo/construtor");
	}

	paramListOpt(); // Chama a Regra 17 (opcional)

	// "match" do token ')'
	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_RPAREN) {
		advance();
	} else {
		error("Esperado ')' para fechar a lista de parametros");
	}

	// "match" do token '{'
	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LBRACE) {
		advance();
	} else {
		error("Esperado '{' para iniciar o bloco de statements");
	}

	statementsOpt(); // Chama a Regra 20 (opcional)

	// "match" do token '}'
	if (lToken->name == SEP && lToken->attribute == ATTR_SEP_RBRACE) {
		advance();
	} else {
		error("Esperado '}' para fechar o bloco de statements");
	}
}

void
Parser::paramListOpt()
{
	// Regra 19: ParamList começa com um 'Type' (int, string, ID) [cite: 72-74]
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
	{
		paramList(); // <-- Vamos implementar isto a seguir
	}
	// Se não (ε - vazio), não faz nada.
}

void
Parser::paramList()
{
	// A lógica do paramListOpt() já garantiu que temos
	// pelo menos um 'Type', então chamamos param() uma vez.
	param();

	// Agora, verificamos se há *mais* parâmetros
	// (enquanto o token for ',')
	while (lToken->name == SEP && lToken->attribute == ATTR_SEP_COMMA)
	{
		advance(); // Consome a ','
		param();   // Chama o próximo parâmetro
	}
}

void
Parser::param()
{
	type();    // Chama a função 'type' (Regra 9, que já temos)
	match(ID); // Consome o ID do parâmetro
}

void
Parser::statementsOpt()
{
    // Esta verificação é complexa. Se o token NÃO for '}',
    // assumimos que há statements.
	if (lToken->name != SEP || lToken->attribute != ATTR_SEP_RBRACE)
	{
		statements(); // <-- Vamos implementar isto a seguir
	}
	// Se não (ε - vazio), não faz nada.
}

void
Parser::statements()
{
	// O statementsOpt() garantiu que temos pelo menos um statement.
	// Chamamos statement() uma vez.
	statement();

	// Agora, continuamos a consumir statements enquanto
	// o token NÃO for o '}' de fecho do methodBody.
	while (lToken->name != SEP || lToken->attribute != ATTR_SEP_RBRACE)
	{
		statement();
	}
}

void
Parser::statement()
{
	// Esta função é um grande "switch" que decide
	// qual regra de statement seguir, com base no token atual.
	
	switch (lToken->name)
	{
		// Regra 22: Statement → VarDeclList
		// (VarDeclList começa com 'int' ou 'string')
		case INT:
		case STRING:
			varDeclList(); // Reutilizamos a função que já temos!
			break;

		// Regra 22: Statement → AtribStat ;
		// (AtribStat começa com ID. Tambem tratamos o conflito
		// de VarDeclList (Type ID) aqui)
		case ID:
			// Por agora, vamos assumir que ID começa um AtribStat
			atribStat();
			match(SEP, ATTR_SEP_SEMICOLON);
			break;
		
		// Regra 22: Statement → PrintStat ;
		case PRINT:
			printStat();
			match(SEP, ATTR_SEP_SEMICOLON);
			break;
		
		// Regra 22: Statement → ReadStat ;
		case READ:
			readStat();
			match(SEP, ATTR_SEP_SEMICOLON);
			break;

		// Regra 22: Statement → ReturnStat ;
		case RETURN:
			returnStat();
			match(SEP, ATTR_SEP_SEMICOLON);
			break;

		// Regra 22: Statement → SuperStat ;
		case SUPER:
			superStat();
			match(SEP, ATTR_SEP_SEMICOLON);
			break;

		// Regra 22: Statement → IfStat
		case IF:
			ifStat();
			break;

		// Regra 22: Statement → ForStat
		case FOR:
			forStat();
			break;

		// Regra 22: Statement → break ;
		case BREAK:
			advance(); // Consome 'break'
			match(SEP, ATTR_SEP_SEMICOLON); // Consome ';'
			break;

		case SEP:
			if (lToken->attribute == ATTR_SEP_SEMICOLON) {
				// Regra 22: Statement → ; (statement vazio)
				advance();
			} 
			else if (lToken->attribute == ATTR_SEP_LBRACE) {
				// Regra 22: Statement → { Statements } (novo escopo)
				advance(); // Consome '{'
				statements(); // Chama a lista de statements
				match(SEP, ATTR_SEP_RBRACE); // Consome '}'
			} else {
				error("Statement inesperado iniciado com este separador");
			}
			break;

		default:
			error("Statement inesperado");
	}
}


// --- STUBS (Placeholders) para os Statements ---

void Parser::atribStat()
{
	// 1. LValue
	// O roteador 'statement()' já garantiu que o token é ID.
	// O 'lValue()' consome o ID e qualquer "cauda" (como .campo ou [indice]).
	lValue(); // Regra 32
	
	// 2. = (Atribuição)
	match(OP, ATTR_OP_ATTR); // Consome '='

	// 3. Decidir entre Expression ou AllocExpression
	// Olhamos para o próximo token:
	if (lToken->name == NEW) {
		// É uma AllocExpression
		allocExpression(); // Regra 35
	} else {
		// É uma Expression normal
		expression(); // Regra 34
	}
}

// Regra 24: PrintStat → print Expression
void Parser::printStat()
{
	match(PRINT);
	// ... (expression)
	cout << "Placeholder: printStat() foi chamado." << endl;
}

// Regra 25: ReadStat → read LValue
void Parser::readStat()
{
	match(READ);
	// lValue();
	cout << "Placeholder: readStat() foi chamado." << endl;
}

// Regra 26: ReturnStat → return Expression
void Parser::returnStat()
{
	match(RETURN);
	// ... (expression)
	cout << "Placeholder: returnStat() foi chamado." << endl;
}

// Regra 27: SuperStat → super ( ArgListOpt )
void Parser::superStat()
{
	match(SUPER);
	// match(SEP, ATTR_SEP_LPAREN);
	// ... (argListOpt)
	// match(SEP, ATTR_SEP_RPAREN);
	cout << "Placeholder: superStat() foi chamado." << endl;
}

// Regra 28: IfStat → if ( Expression ) { Statements } [ else { Statements } ]
void Parser::ifStat()
{
	// 1. Parte 'if'
	match(IF); // Consome 'if'
	
	match(SEP, ATTR_SEP_LPAREN); // Consome '('
	expression(); // Regra 34
	match(SEP, ATTR_SEP_RPAREN); // Consome ')'

	match(SEP, ATTR_SEP_LBRACE); // Consome '{'
	
	// A gramática original [cite: 105] usa 'Statements' (plural),
	// o que significa que o { } é obrigatório e pode conter
	// múltiplos statements.
	statements(); // Regra 21
	
	match(SEP, ATTR_SEP_RBRACE); // Consome '}'

	// 2. Parte 'else' (Opcional)
	if (lToken->name == ELSE)
	{
		advance(); // Consome 'else'
		
		match(SEP, ATTR_SEP_LBRACE); // Consome '{'
		statements(); // Regra 21
		match(SEP, ATTR_SEP_RBRACE); // Consome '}'

		if (lToken->name == ELSE)
		{
			advance(); // Consome 'else'
			match(SEP, ATTR_SEP_LBRACE); // Consome '{'
			statementsOpt(); // Regra 20
			match(SEP, ATTR_SEP_RBRACE); // Consome '}'
		}
	}
	// Se não houver 'else', a função termina.
}

// Regra 29: ForStat → for ( AtribStatOpt ExpressionOpt AtribStatOpt ) { Statements }
void Parser::forStat()
{
	match(FOR); // Consome 'for'

	match(SEP, ATTR_SEP_LPAREN); // Consome '('

	// 1. AtribStatOpt (Inicializador)
	atribStatOpt(); // Regra 30
	match(SEP, ATTR_SEP_SEMICOLON); // Consome ';'

	// 2. ExpressionOpt (Condição)
	expressionOpt(); // Regra 31
	match(SEP, ATTR_SEP_SEMICOLON); // Consome ';'

	// 3. AtribStatOpt (Incremento)
	atribStatOpt(); // Regra 30

	match(SEP, ATTR_SEP_RPAREN); // Consome ')'

	// Bloco de Statements
	match(SEP, ATTR_SEP_LBRACE); // Consome '{'
	
	// Usamos statementsOpt para permitir corpos vazios {}
	statementsOpt(); // Regra 20
	
	match(SEP, ATTR_SEP_RBRACE); // Consome '}'
}

void Parser::atribStatOpt()
{
	// Como sabemos se um AtribStat (Regra 23) [cite: 94-96] começa?
	// Ele começa com um LValue (Regra 32)[cite: 111], que começa com ID.
	if (lToken->name == ID)
	{
		atribStat(); // Chama a Regra 23
	}
	// Se não (ε - vazio), não faz nada.
}

/**
 * @brief Implementa a Regra 31: ExpressionOpt → Expression | ε
 * (Auxiliar do ForStat)
 */
void Parser::expressionOpt()
{
	// Como sabemos se uma Expression (Regra 34) [cite: 114-115] começa?
	// Olhamos os "FIRST" de Factor (Regra 39) [cite: 129-133]:
	// ID, INTEGER_LITERAL, STRING_LITERAL, (
	if (lToken->name == ID || lToken->name == INTEGER_LITERAL ||
		lToken->name == STRING_LITERAL || 
		(lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN))
	{
		expression(); // Chama a Regra 34
	}
	// Se não (ε - vazio), não faz nada.
}

// Precisamos de uma nova função 'match' para atributos
void
Parser::match(int t, int attr)
{
	if (lToken->name == t && lToken->attribute == attr)
		advance();
	else
		error("Erro inesperado (esperava atributo)");
}

void Parser::readStat()
{
	match(READ);
	lValue(); // Chama a Regra 32
}

/**
 * @brief Implementa a Regra 24: PrintStat → print Expression
 */
void Parser::printStat()
{
	match(PRINT);
	expression(); // Chama a Regra 34
}

// === PILHA DE EXPRESSÕES (Regras 32-41) ===

/**
 * @brief Implementa a Regra 32: LValue → ID LValueComp
 */
void Parser::lValue()
{
	match(ID);
	lValueComp(); // Chama a Regra 33
}

/**
 * @brief Implementa a Regra 33 (iterativa):
 * LValueComp → { . ID LValueComp | . ID ( ArgListOpt ) LValueComp | [ Expression ] LValueComp }
 */
void Parser::lValueComp()
{
	// Esta regra é uma "cauda" opcional, implementada com um loop
	while (true) {
		if (lToken->name == SEP && lToken->attribute == ATTR_SEP_DOT) {
			// . ID LValueComp
			// . ID ( ArgListOpt ) LValueComp
			advance(); // Consome '.'
			match(ID);

			if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN) {
				// É uma chamada de método: . ID ( ArgListOpt )
				advance(); // Consome '('
				argListOpt();
				match(SEP, ATTR_SEP_RPAREN); // Consome ')'
			}
			// Continua o loop para o próximo LValueComp

		} else if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LBRACKET) {
			// [ Expression ] LValueComp
			advance(); // Consome '['
			expression();
			match(SEP, ATTR_SEP_RBRACKET); // Consome ']'
			// Continua o loop para o próximo LValueComp

		} else {
			// Produção vazia (ε), fim da cauda
			return; 
		}
	}
}

/**
 * @brief Implementa a Regra 34:
 * Expression → NumExpression | NumExpression RelOp NumExpression
 */
void Parser::expression()
{
	numExpression(); // Chama a Regra 36

	// Verifica se há um RelOp
	if (lToken->name == OP && (
		lToken->attribute == ATTR_OP_LT || lToken->attribute == ATTR_OP_GT ||
		lToken->attribute == ATTR_OP_LE || lToken->attribute == ATTR_OP_GE ||
		lToken->attribute == ATTR_OP_EQ || lToken->attribute == ATTR_OP_NE
	))
	{
		advance(); // Consome o RelOp
		numExpression(); // Chama a Regra 36 novamente
	}
}

/**
 * @brief Implementa a Regra 36 (Refatorada):
 * NumExpression → Term NumExpressionLinha
 */
void Parser::numExpression()
{
	term();
	numExpressionLinha();
}

/**
 * @brief Implementa a Regra 36 (Refatorada):
 * NumExpressionLinha → + Term NumExpressionLinha | - Term NumExpressionLinha | ε
 */
void Parser::numExpressionLinha()
{
	if (lToken->name == OP && lToken->attribute == ATTR_OP_PLUS) {
		advance(); // Consome '+'
		term();
		numExpressionLinha();
	} else if (lToken->name == OP && lToken->attribute == ATTR_OP_MINUS) {
		advance(); // Consome '-'
		term();
		numExpressionLinha();
	}
	// Se não (ε - vazio), não faz nada.
}

/**
 * @brief Implementa a Regra 37 (Refatorada):
 * Term → UnaryExpression TermLinha
 */
void Parser::term()
{
	unaryExpression();
	termLinha();
}

/**
 * @brief Implementa a Regra 37 (Refatorada):
 * TermLinha → * UnaryExpression TermLinha | ... | ε
 */
void Parser::termLinha()
{
	if (lToken->name == OP && lToken->attribute == ATTR_OP_MULT) {
		advance(); // Consome '*'
		unaryExpression();
		termLinha();
	} else if (lToken->name == OP && lToken->attribute == ATTR_OP_DIV) {
		advance(); // Consome '/'
		unaryExpression();
		termLinha();
	} else if (lToken->name == OP && lToken->attribute == ATTR_OP_MOD) {
		advance(); // Consome '%'
		unaryExpression();
		termLinha();
	}
	// Se não (ε - vazio), não faz nada.
}

/**
 * @brief Implementa a Regra 38:
 * UnaryExpression → + Factor | - Factor | Factor
 */
void Parser::unaryExpression()
{
	if (lToken->name == OP && lToken->attribute == ATTR_OP_PLUS) {
		advance(); // Consome '+'
	} else if (lToken->name == OP && lToken->attribute == ATTR_OP_MINUS) {
		advance(); // Consome '-'
	}
	
	factor(); // Chama a Regra 39
}

/**
 * @brief Implementa a Regra 39:
 * Factor → INTEGER_LITERAL | STRING_LITERAL | LValue | ( Expression )
 */
void Parser::factor()
{
	if (lToken->name == INTEGER_LITERAL) {
		advance();
	} else if (lToken->name == STRING_LITERAL) {
		advance();
	} else if (lToken->name == ID) {
		// Se começa com ID, é um LValue
		lValue();
	} else if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN) {
		// ( Expression )
		advance(); // Consome '('
		expression();
		match(SEP, ATTR_SEP_RPAREN); // Consome ')'
	} else {
		error("Esperado um Fator (numero, string, LValue ou '( Expression )')");
	}
}

/**
 * @brief Implementa a Regra 40: ArgListOpt → ArgList | ε
 */
void Parser::argListOpt()
{
	// Como sabemos se ArgList (Regra 41) [cite: 135-137] começa?
	// Ela começa com Expression (Regra 34) [cite: 114-115].
	// Os inícios de Expression são: ID, INTEGER_LITERAL, STRING_LITERAL, (
	if (lToken->name == ID || lToken->name == INTEGER_LITERAL ||
		lToken->name == STRING_LITERAL || 
		(lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN))
	{
		argList();
	}
	// Se não (ε - vazio), não faz nada.
}

/**
 * @brief Implementa a Regra 41 (Refatorada):
 * ArgList → Expression { , Expression }
 */
void Parser::argList()
{
	expression(); // Consome a primeira expressão

	// Consome { , Expression }
	while (lToken->name == SEP && lToken->attribute == ATTR_SEP_COMMA)
	{
		advance(); // Consome ','
		expression();
	}
}

/**
 * @brief Implementa a Regra 35:
 * AllocExpression → new ID ( ArgListOpt ) | new Type [ Expression ]
 */
void Parser::allocExpression()
{
	// 1. Consome 'new'
	match(NEW);

	// 2. Decidir entre 'new ID' ou 'new Type'
	// 'Type' pode ser 'int', 'string' ou 'ID' (Regra 9) [cite: 48, 50-51]
	
	if (lToken->name == ID) 
	{
		// O token é ID. Pode ser 'new ID ()' OU 'new ID []'.
		advance(); // Consome o ID (que está a ser usado como Type)

		if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LPAREN) {
			// É: new ID ( ArgListOpt )
			advance(); // Consome '('
			argListOpt(); // Regra 40 (já temos)
			match(SEP, ATTR_SEP_RPAREN); // Consome ')'
		} 
		else if (lToken->name == SEP && lToken->attribute == ATTR_SEP_LBRACKET) {
			// É: new Type [ Expression ] (onde Type era um ID)
			advance(); // Consome '['
			expression(); // Regra 34 (já temos)
			match(SEP, ATTR_SEP_RBRACKET); // Consome ']'
		} else {
			error("Esperado '(' ou '[' apos 'new ID' na alocacao");
		}
	} 
	else if (lToken->name == INT || lToken->name == STRING) 
	{
		// É: new Type [ Expression ] (onde Type é 'int' ou 'string')
		advance(); // Consome 'int' ou 'string'
		
		match(SEP, ATTR_SEP_LBRACKET); // Consome '['
		expression(); // Regra 34
		match(SEP, ATTR_SEP_RBRACKET); // Consome ']'
	} 
	else 
	{
		error("Esperado um tipo (ID, 'int' ou 'string') apos 'new'");
	}
}

void
Parser::methodDeclListOpt()
{
    // Se o token for 'int', 'string' ou 'ID', pode ser um método.
    // (Vamos refinar isto depois)
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
	{
		methodDeclList(); // <-- Próximo passo
	}
	// Se não (ε - vazio), não fazemos nada.
}


void
Parser::methodDeclListOpt()
{
	// Regra 15: MethodDecl começa com 'Type' (int, string, ID)
	// Como isto é chamado *depois* de varDeclListOpt e constructDeclListOpt,
	// qualquer 'Type' encontrado aqui deve ser o início de um método.
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
	{
		methodDeclList(); // <-- Ativamos esta função!
	}
	// Se não (ε - vazio), não fazemos nada.
}

void
Parser::methodDeclList()
{
	// O methodDeclListOpt() já garantiu que temos
	// pelo menos um 'Type', então chamamos methodDecl() uma vez.
	do
	{
		methodDecl();
	}
	// Continua enquanto o próximo token for um 'Type'
	// (início de outro método)
	while(lToken->name == INT || lToken->name == STRING || lToken->name == ID);
}

void
Parser::methodDecl()
{
	// 1. Reutiliza a função 'type' (Regra 9)
	type();
	
	// 2. Consome o ID do método
	match(ID); 
	
	// 3. Reutiliza a função 'methodBody' (Regra 16)
	// que já implementámos para os construtores!
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
