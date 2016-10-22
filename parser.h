#include <vector>
#include "lexer.h"

/**
 * Brainfuck grammar:
 * Program = Expression*
 * Expression = Loop | Operator
 * Loop = LPAR Expression* RPAR
 * Operator = MOVE_RIGHT | MOVE_LEFT | INCREMENT | DECREMENT | WRITE | READ
 */
class ASTExpression {
public:
	ASTExpression();
	virtual ~ASTExpression();
	virtual void print() = 0;
};

class ASTLoop : public ASTExpression {
private:
	std::vector<ASTExpression*> expressions;
public:
	ASTLoop();
	~ASTLoop();
	std::vector<ASTExpression*>* getExpressions();
	void addExpression(ASTExpression* expression);
	void print();
};

class ASTOperator : public ASTExpression {
private:
	Token token;
public:
	ASTOperator(Token _token);
	~ASTOperator();
	Token getToken();
	void print();
};

class ASTProgram {
private:
	std::vector<ASTExpression*> expressions;
public:
	ASTProgram();
	~ASTProgram();
	std::vector<ASTExpression*>* getExpressions();
	void addExpression(ASTExpression* expression);
	void print();
};

/**
 * This is a very simple recursive decent parser without error handling.
 */
class Parser {
private:
	unsigned int currentPosition;
	std::vector<Token>* tokens;
	ASTExpression* parseExpression();
	ASTLoop* parseLoop();
	ASTOperator* parseOperator();
public:
	Parser();
	~Parser();
	ASTProgram* parse(std::vector<Token>* _tokens);
};