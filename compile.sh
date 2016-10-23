#!
./brainfuck_compiler $1 $2 > $3.s
g++ $3.s -o $3