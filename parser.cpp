#include "parser.h"
#include <iostream>

ASTProgram::ASTProgram() {}

ASTProgram::~ASTProgram() {
	for (unsigned int i=0; i<expressions.size(); i++) {
		delete expressions[i];
	}
}

void ASTProgram::addExpression(ASTExpression* expression) {
	expressions.push_back(expression);
}

std::vector<ASTExpression*>* ASTProgram::getExpressions() {
	return &expressions;
}

void ASTProgram::print() {
	std::cout << "Brainfuck Program: " << std::endl;
	for (unsigned int i=0; i<expressions.size(); i++) {
		(expressions[i])->print();
	}
	std::cout << std::endl;
}

ASTExpression::ASTExpression() {}
ASTExpression::~ASTExpression() {}

ASTLoop::ASTLoop() {}
ASTLoop::~ASTLoop() {
	for (unsigned int i=0; i<expressions.size(); i++) {
		delete expressions[i];
	}
}

void ASTLoop::addExpression(ASTExpression* expression) {
	expressions.push_back(expression);
}

std::vector<ASTExpression*>* ASTLoop::getExpressions() {
	return &expressions;
}

void ASTLoop::print() {
	std::cout << "[";
	for (unsigned int i=0; i<expressions.size(); i++) {
		(expressions[i])->print();
	}
	std::cout << "]";
}

ASTOperator::ASTOperator(Token _token) {
	token = _token;
}

ASTOperator::~ASTOperator() {}

Token ASTOperator::getToken() {
	return token;
}

void ASTOperator::print() {
	switch(token) {
		case MOVE_RIGHT:
			std::cout << ">";
			break;
		case MOVE_LEFT:
			std::cout << "<";
			break;
		case INCREMENT:
			std::cout << "+";
			break;
		case DECREMENT:
			std::cout << "-";
			break;
		case WRITE:
			std::cout << ".";
			break;
		case READ:
			std::cout << ",";
			break;
		default:
			std::cout << "?";
			break;
	}
}

Parser::Parser() {}
Parser::~Parser() {}

ASTProgram* Parser::parse(std::vector<Token>* _tokens) {
	currentPosition = 0;
	tokens = _tokens;
	ASTProgram* program = new ASTProgram();

	while (currentPosition < tokens->size()) {
		program->addExpression(parseExpression());
	}

	return program;
}


ASTExpression* Parser::parseExpression() {
	if ((*tokens)[currentPosition] == LPAR) {
		// this is a loop
		return parseLoop();
	}
	else {
		// this is an operator
		return parseOperator();
	}
}

ASTLoop* Parser::parseLoop() {
	currentPosition++; // LPAR

	ASTLoop* loop = new ASTLoop();
	while ((*tokens)[currentPosition] != RPAR) {
		loop->addExpression(parseExpression());
	}

	currentPosition++; // RPAR
	return loop;
}

ASTOperator* Parser::parseOperator() {
	ASTOperator* op = new ASTOperator((*tokens)[currentPosition]);
	currentPosition++; // The operator
	return op;
}