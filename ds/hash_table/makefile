all: debug release

debug: hash_table_debug.out

release: hash_table_release.out

hash_table_release.out: hash_table_release.o hash_table_test.o
	gcc -o hash_table_release.out hash_table_release.o hash_table_test.o
	
hash_table_debug.out: hash_table_debug.o hash_table_test.o
	gcc -o hash_table_debug.out hash_table_debug.o hash_table_test.o -I ../../ds/dlist -L/home/maya/maya-petter/projects/hash_table -ldlist
	
hash_table_test.o: hash_table_test.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c hash_table_test.c
	
hash_table_debug.o: hash_table.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c hash_table.c -o hash_table_debug.o
	
hash_table_release.o: hash_table.c
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c hash_table.c -I ../../ds/dlist ../../ds/dlist.h -L$(LD_LIBRARY_PATH) -ldlist -o hash_table_release.o
