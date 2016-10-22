#include "parser.h"

class Compiler {
private:
	ASTProgram* program;
	int labelCounter;
	void compileExpression(ASTExpression* expression);
	void compileLoop(ASTLoop* loop);
	void compileOperator(ASTOperator* op);
public:
	Compiler();
	~Compiler();
	void compile(ASTProgram* _program, int tapeSize);
};