#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "compiler.h"

using namespace std;

/**
 * A simple brainfuck compiler.
 * The arguments are as follows:
 *  1. the input filename
 *  2. the tape size
 */
int main(int argc, char **argv) {
	// check arguments
	if (argc != 3) {
		cout << "Error: Invalid syntax. Please state the input filename and the tape size." << endl;
		return EXIT_FAILURE;
	}

	// open the file
	char* sourceFilename = argv[1];
	ifstream* sourceFile = new ifstream(sourceFilename);
	if (!sourceFile->good()) {
		cerr << "Error: Could not read the file." << endl;
		return EXIT_FAILURE;
	}

	// read the tape size
	istringstream ss(argv[2]);
	int tapeSize;
	if (!(ss >> tapeSize)) {
	    cerr << "Error: Invalid tape size." << endl;
	}

	// compile the brainfuck code
	Lexer lexer;
	vector<Token>* tokens = lexer.lex(sourceFile);
	sourceFile->close();

	Parser parser;
	ASTProgram* program = parser.parse(tokens);

	Compiler compiler;
	compiler.compile(program, tapeSize);

	delete program;
	delete tokens;
	return EXIT_SUCCESS;
}