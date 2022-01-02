all: debug release

debug: java2c_debug.out

release: java2c_release.out

java2c_release.out: java2c_release.o java2c_test.o
	gcc -o java2c_release.out java2c_release.o java2c_test.o
	
java2c_debug.out: java2c_debug.o java2c_test.o
	gcc -o java2c_debug.out java2c_debug.o java2c_test.o
	
java2c_test.o: java2c_test.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c java2c_test.c
	
java2c_debug.o: java2c.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c java2c.c -o java2c_debug.o
	
java2c_release.o: java2c.c
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c java2c.c -o java2c_release.o
