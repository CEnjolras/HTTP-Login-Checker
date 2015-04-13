pwt: main.o connect.o easy_string.o bddParser.o
	gcc -o pwt main.o connect.o easy_string.o bddParser.o `curl-config --libs`

main.o: main.c connect.h
	gcc -c main.c -o main.o

connect.o: connect.c connect.h
	gcc -c connect.c -o connect.o

easy_string.o: easy_string.c easy_string.h
	gcc -c easy_string.c -o easy_string.o

bddParser.o: bddParser.c bddParser.h
	gcc -c bddParser.c -o bddParser.o
