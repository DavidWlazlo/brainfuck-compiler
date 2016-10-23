#include <iostream>
#include <stack>

using namespace std;

const int TAPE_SIZE = 4000;

/**
 * A simple brainfuck compiler.
 * The arguments are as follows:
 *  1. the input string
 * All other arguments are simply ignored.
 */
int main(int argc, char **argv) {
	if(argc < 2) {
		return 0;
	}

	cout << "\t.globl\ttape\n"
	     << "\t.bss\n"
	     << "\t.align 32\n"
	     << "\t.type\ttape, @object\n"
     	 << "\t.size\ttape, " << TAPE_SIZE << "\n"
    << "tape:\n"
	     << "\t.zero\t" << TAPE_SIZE << "\n"
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

	char* program = argv[1];
	int lastLoopID = 0;
	int currentLoopID = 0;
	stack<int> loopStack;
	while (*program != 0) {
		switch(*program) {
		case '>':
			cout << "\tmovq\tptr(%rip), %rax\n"
	             << "\taddq\t$1, %rax\n"
	             << "\tmovq\t%rax, ptr(%rip)\n";
			break;
		case '<':
			cout << "\tmovq\tptr(%rip), %rax\n"
	             << "\tsubq\t$1, %rax\n"
	             << "\tmovq\t%rax, ptr(%rip)\n";
			break;
		case '+':
			cout << "\tmovq\tptr(%rip), %rax\n"
	             << "\tmovzbl\t(%rax), %edx\n"
	             << "\taddl\t$1, %edx\n"
	             << "\tmovb\t%dl, (%rax)\n";
			break;
		case '-':
			cout << "\tmovq\tptr(%rip), %rax\n"
	             << "\tmovzbl\t(%rax), %edx\n"
	             << "\tsubl\t$1, %edx\n"
	             << "\tmovb\t%dl, (%rax)\n";
			break;
		case '.':
			cout << "\tmovq\tptr(%rip), %rax\n"
	             << "\tmovzbl\t(%rax), %eax\n"
	             << "\tmovsbl\t%al, %eax\n"
	             << "\tmovl\t%eax, %edi\n"
	             << "\tcall\tputchar\n";
			break;
		case ',':
			cout << "\tmovq\tptr(%rip), %rbx\n"
	             << "\tcall\tgetchar\n"
	             << "\tmovb\t%al, (%rbx)\n";
			break;
		case '[':
			lastLoopID++;
			cout << ".lpar" << lastLoopID << ":\n"
			     << "\tmovq\tptr(%rip), %rax\n"
			     << "\tmovzbl\t(%rax), %eax\n"
			     << "\ttestb\t%al, %al\n"
			     << "\tje\t.rpar" << lastLoopID << "\n";
			loopStack.push(lastLoopID);
			break;
		case ']':
			currentLoopID = loopStack.top();
			cout << "\tjmp\t.lpar" << currentLoopID  << "\n"
	     	     << ".rpar" << currentLoopID  << ":\n";
			loopStack.pop();
			break;
		default:
			break;
		}

		program++;
	}

	// end of program
	cout << "\tmovl\t$0, %eax\n"
	     << "\taddq\t$8, %rsp\n"
	     << "\tpopq\t%rbx\n"
	     << "\tpopq	%rbp\n"
	     << "\t.cfi_def_cfa 7, 8\n"
	     << "\tret\n"
	     << "\t.cfi_endproc" << endl;

	return 0;
}