OBJS = brainfuck_compiler.o lexer.o parser.o compiler.o
CC = g++
DEBUG = -g -Wall
CFLAGS = -O2 -c -std=gnu++11 $(DEBUG)
LFLAGS = $(Debug)

brainfuck_compiler: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o brainfuck_compiler

brainfuck_compiler.o: brainfuck_compiler.cpp lexer.h parser.h
	$(CC) $(CFLAGS) brainfuck_compiler.cpp

lexer.o: lexer.cpp lexer.h
	$(CC) $(CFLAGS) lexer.cpp

parser.o: parser.cpp parser.h lexer.h
	$(CC) $(CFLAGS) parser.cpp

compiler.o: compiler.cpp compiler.h parser.h
	$(CC) $(CFLAGS) compiler.cpp

clean:
	\rm *.o brainfuck_compiler