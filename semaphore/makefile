all: ping pong

ping: ping.out

pong: pong.out

pong.out: pong.o 
	gcc -o pong.out pong.o 
	
ping.out: ping.o 
	gcc -o ping.out ping.o 
	
ping.o: ping.c
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c ping.c -o ping.o
	
pong.o: pong.c
	gcc -ansi -pedantic-errors -Wall -Wextra -DNping -O3 -c pong.c -o pong.o
