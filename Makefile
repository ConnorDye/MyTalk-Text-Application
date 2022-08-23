CC = gcc
CFLAGS = -Wall -g -pedantic -std=gnu99 -I ~pn-cs357/Given/Talk/include
LD = gcc 
LDFLAGS = -g -Wall -L
                                                                                
mytalk: mytalk.o tcp.o
	$(LD) $(LDFLAGS) ~pn-cs357/Given/Talk/lib64 -o mytalk mytalk.o tcp.o -ltalk -lncurses

mytalk.o: mytalk.c
	$(CC) $(CFLAGS) -c -o mytalk.o mytalk.c

tcp.o: tcp.c
	$(CC) $(CFLAGS) -c -o tcp.o tcp.c

clean:
	rm *.o mytalk

scp:
	scp connection.h mytalk.c tcp.c talk.h libtalk.a README Makefile chdye@unix2.csc.calpoly.edu:./asgn5

git: 
	git add mytalk.c tcp.c connection.h libtalk.a talk.h README Makefile
	git commit -m "$m" 
	git push -u origin server

gdb:
	gdb --args ./mytalk -v 2062
