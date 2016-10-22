#include "compiler.h"
#include <iostream>

using namespace std;

Compiler::Compiler() {}

Compiler::~Compiler() {}

void Compiler::compile(ASTProgram* _program, int tapeSize) {
	program = _program;
	labelCounter = 0;

	// start of program
	cout << "\t.globl\ttape\n"
	     << "\t.bss\n"
	     << "\t.align 32\n"
	     << "\t.type\ttape, @object\n"
     	 << "\t.size\ttape, " << tapeSize << "\n"
    << "tape:\n"
	     << "\t.zero\t" << tapeSize << "\n"
	     << "\t.globl\tptr\n"
	     << "\t.data\n"
	     << "\t.align 8\n"
	     << "\t.type\tptr, @object\n"
	     << "\t.size\tptr, 8\n"
    << "ptr:\n"
	     << "\t.quad\ttape\n"
	     << "\t.text\n"
	     << "\t.globl\tmain\n"
	     << "\t.type\tmain, @function\n"
    << "main:\n"
	     << "\t.cfi_startproc\n"
	     << "\tpushq\t%rbp\n"
	     << "\t.cfi_def_cfa_offset 16\n"
	     << "\t.cfi_offset 6, -16\n"
	     << "\tmovq\t%rsp, %rbp\n"
	     << "\t.cfi_def_cfa_register 6\n"
	     << "\tpushq\t%rbx\n"
	     << "\tsubq\t$8, %rsp\n"
	     << "\t.cfi_offset 3, -24\n";

	// compile the expressions
	for(unsigned int i=0; i<program->getExpressions()->size(); i++) {
		compileExpression((*(program->getExpressions()))[i]);
	}

	// end of program
	cout << "\tmovl\t$0, %eax\n"
	     << "\taddq\t$8, %rsp\n"
	     << "\tpopq\t%rbx\n"
	     << "\tpopq	%rbp\n"
	     << "\t.cfi_def_cfa 7, 8\n"
	     << "\tret\n"
	     << "\t.cfi_endproc" << endl;
}

void Compiler::compileExpression(ASTExpression* expression) {
	if (ASTLoop* loop = dynamic_cast<ASTLoop*>(expression))
	{
	    compileLoop(loop);
	}
	else if(ASTOperator* op = dynamic_cast<ASTOperator*>(expression))
	{
	    compileOperator(op);
	}
}

void Compiler::compileLoop(ASTLoop* loop) {
	labelCounter++;
	int thisLabelCounter = labelCounter;

	cout << ".lpar" << thisLabelCounter << ":\n"
	     << "\tmovq\tptr(%rip), %rax\n"
	     << "\tmovzbl\t(%rax), %eax\n"
	     << "\ttestb\t%al, %al\n"
	     << "\tje\t.rpar" << thisLabelCounter << "\n";

	// compile the expressions
	for(unsigned int i=0; i<loop->getExpressions()->size(); i++) {
		compileExpression((*(loop->getExpressions()))[i]);
	}

	cout << "\tjmp\t.lpar" << thisLabelCounter << "\n"
	     << ".rpar" << thisLabelCounter << ":\n";
}

void Compiler::compileOperator(ASTOperator* op) {
	switch(op->getToken()) {
		case MOVE_RIGHT:
			cout << "\tmovq\tptr(%rip), %rax\n"
	             << "\taddq\t$1, %rax\n"
	             << "\tmovq\t%rax, ptr(%rip)\n";
			break;
		case MOVE_LEFT:
			cout << "\tmovq\tptr(%rip), %rax\n"
	             << "\tsubq\t$1, %rax\n"
	             << "\tmovq\t%rax, ptr(%rip)\n";
			break;
		case INCREMENT:
			cout << "\tmovq\tptr(%rip), %rax\n"
	             << "\tmovzbl\t(%rax), %edx\n"
	             << "\taddl\t$1, %edx\n"
	             << "\tmovb\t%dl, (%rax)\n";
			break;
		case DECREMENT:
			cout << "\tmovq\tptr(%rip), %rax\n"
	             << "\tmovzbl\t(%rax), %edx\n"
	             << "\tsubl\t$1, %edx\n"
	             << "\tmovb\t%dl, (%rax)\n";
			break;
		case WRITE:
			cout << "\tmovq\tptr(%rip), %rax\n"
	             << "\tmovzbl\t(%rax), %eax\n"
	             << "\tmovsbl\t%al, %eax\n"
	             << "\tmovl\t%eax, %edi\n"
	             << "\tcall\tputchar\n";
			break;
		case READ:
			cout << "\tmovq\tptr(%rip), %rbx\n"
	             << "\tcall\tgetchar\n"
	             << "\tmovb\t%al, (%rbx)\n";
			break;
		default:
			break;
	}
}