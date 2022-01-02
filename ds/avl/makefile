all: debug release

debug: avl_debug.out

release: avl_release.out

avl_release.out: avl_release.o avl_test.o
	gcc -o avl_release.out avl_release.o avl_test.o
	
avl_debug.out: avl_debug.o avl_test.o
	gcc -o avl_debug.out avl_debug.o avl_test.o
	
avl_test.o: avl_test.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c avl_test.c
	
avl_debug.o: avl.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c avl.c -o avl_debug.o
	
avl_release.o: avl.c
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c avl.c -o avl_release.o
