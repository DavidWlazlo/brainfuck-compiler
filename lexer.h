#include <fstream>
#include <vector>

/**
 * Relevant tokens for a brainfuck program.
 */
enum Token {
	MOVE_RIGHT,
	MOVE_LEFT,
	INCREMENT,
	DECREMENT,
	WRITE,
	READ,
	LPAR,
	RPAR
};

class Lexer {
public:
	Lexer();
	~Lexer();
	std::vector<Token>* lex(std::ifstream* file);
};