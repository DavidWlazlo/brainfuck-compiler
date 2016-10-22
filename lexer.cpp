#include "lexer.h"

Lexer::Lexer() {}
Lexer::~Lexer() {}

/**
 * Lexes the input file and returns a list of tokens.
 */
std::vector<Token>* Lexer::lex(std::ifstream* file) {
	std::vector<Token>* token = new std::vector<Token>();

	// read the file charwise and return the corresponding token
	// ignore every other character.
	char c;
	while (file->get(c)) {
		switch(c) {
			case '<':
				token->push_back(MOVE_LEFT);
				break;
			case '>':
				token->push_back(MOVE_RIGHT);
				break;
			case '+':
				token->push_back(INCREMENT);
				break;
			case '-':
				token->push_back(DECREMENT);
				break;
			case '.':
				token->push_back(WRITE);
				break;
			case ',':
				token->push_back(READ);
				break;
			case '[':
				token->push_back(LPAR);
				break;
			case ']':
				token->push_back(RPAR);
				break;
			default:
				break;
		}
	}

	return token;
}