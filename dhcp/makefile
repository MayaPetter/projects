all: debug release

debug: dhcp_debug.out

release: dhcp_release.out

dhcp_release.out: dhcp_release.o dhcp_test.o 
	gcc -o dhcp_release.out dhcp_release.o dhcp_test.o 
	
dhcp_debug.out: dhcp_debug.o dhcp_test.o 
	gcc -o dhcp_debug.out dhcp_debug.o dhcp_test.o 
	
dhcp_test.o: dhcp_test.c 
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c dhcp_test.c 
	
dhcp_debug.o: dhcp.c 
	gcc -ansi -pedantic-errors -Wall -Wextra -g -c dhcp.c -o dhcp_debug.o
	
dhcp_release.o: dhcp.c 
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c dhcp.c -o dhcp_release.o
