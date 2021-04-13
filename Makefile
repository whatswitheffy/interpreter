g++ -c const.cpp -I./include -o const.o
g++ -c lexem.cpp -I./include -o lexem.o
g++ -c lexical.cpp -I./include -o lexical.o
g++ -c syntax.cpp -I./include -o syntax.o
g++ -c semantic.cpp -I./include -o semantic.o
g++ main.cpp const.o lexem.o lexical.o syntax.o semantic.o -I./include -o main
