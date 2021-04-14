all:
	mkdir bin -p
	g++ -c src/const.cpp -I./include -o bin/const.o
	g++ -c src/lexem.cpp -I./include -o bin/lexem.o
	g++ -c src/lexical.cpp -I./include -o bin/lexical.o
	g++ -c src/syntax.cpp -I./include -o bin/syntax.o
	g++ -c src/semantic.cpp -I./include -o bin/semantic.o
	g++ src/main.cpp bin/const.o bin/lexem.o bin/lexical.o bin/syntax.o bin/semantic.o -I./include -o bin/main
