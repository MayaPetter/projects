all: debug release

debug: search_debug.out

release: search_release.out

search_release.out: search_release.o search_test.o
	gcc -o search_release.out search_release.o search_test.o
	
search_debug.out: search_debug.o search_test.o
	gcc -o search_debug.out search_debug.o search_test.o
	
search_test.o: search_test.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c search_test.c
	
search_debug.o: search.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c search.c -o search_debug.o
	
search_release.o: search.c
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c search.c -o search_release.o
