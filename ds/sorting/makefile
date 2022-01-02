all: debug release remove_objs

debug: sorting_debug.out

release: sorting_release.out

sorting_release.out: sorting_release.o sorting_test.o tests.o
	gcc -o sorting_release.out sorting_release.o sorting_test.o tests.o
	
sorting_debug.out: sorting_debug.o sorting_test.o tests.o
	gcc -o sorting_debug.out sorting_debug.o sorting_test.o tests.o
	
sorting_test.o: sorting_test.c tests.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c sorting_test.c
	
sorting_debug.o: sorting.c tests.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c sorting.c -o sorting_debug.o
	
sorting_release.o: sorting.c tests.c
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c sorting.c -o sorting_release.o

remove_objs: $(OBJ_DEPENDENCIES)
	rm *.o
