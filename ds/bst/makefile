all: debug release

debug: bst_debug.out

release: bst_release.out

bst_release.out: bst_release.o bst_test.o
	gcc -o bst_release.out bst_release.o bst_test.o
	
bst_debug.out: bst_debug.o bst_test.o
	gcc -o bst_debug.out bst_debug.o bst_test.o
	
bst_test.o: bst_test.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c bst_test.c
	
bst_debug.o: bst.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c bst.c -o bst_debug.o
	
bst_release.o: bst.c
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c bst.c -o bst_release.o
