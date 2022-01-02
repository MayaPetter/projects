all: debug release

debug: my_shell_debug.out

release: my_shell_release.out

my_shell_release.out: my_shell_release.o my_shell_test.o
	gcc -o my_shell_release.out my_shell_release.o my_shell_test.o
	
my_shell_debug.out: my_shell_debug.o my_shell_test.o
	gcc -o my_shell_debug.out my_shell_debug.o my_shell_test.o
	
my_shell_test.o: my_shell_test.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c my_shell_test.c
	
my_shell_debug.o: my_shell.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c my_shell.c -o my_shell_debug.o
	
my_shell_release.o: my_shell.c
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c my_shell.c -o my_shell_release.o
