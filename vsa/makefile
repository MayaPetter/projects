all: debug release

debug: vsa_debug.out

release: vsa_release.out

vsa_release.out: vsa_release.o vsa_test.o
	gcc -o vsa_release.out vsa_release.o vsa_test.o
	
vsa_debug.out: vsa_debug.o vsa_test.o
	gcc -o vsa_debug.out vsa_debug.o vsa_test.o
	
vsa_test.o: vsa_test.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c vsa_test.c
	
vsa_debug.o: vsa.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c vsa.c -o vsa_debug.o
	
vsa_release.o: vsa.c
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c vsa.c -o vsa_release.o
