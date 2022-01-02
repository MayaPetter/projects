all: debug release

debug: calculator_debug.out

release: calculator_release.out

calculator_release.out: calculator_release.o calculator_test.o
	gcc -o calculator_release.out calculator_release.o calculator_test.o
	
calculator_debug.out: calculator_debug.o calculator_test.o
	gcc -o calculator_debug.out calculator_debug.o calculator_test.o -I ../../ds/stack -L/home/maya/maya-petter/projects/calculator -lstack -lm
	
calculator_test.o: calculator_test.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c calculator_test.c
	
calculator_debug.o: calculator.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c calculator.c -o calculator_debug.o
	
calculator_release.o: calculator.c
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c calculator.c -I ../../ds/stack ../../ds/stack.h -L$(LD_LIBRARY_PATH) -lstack -o calculator_release.o
