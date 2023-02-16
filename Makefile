.PHONY: all clean
all: main_test
main_test: 
	gcc -g -std=c11 -D_GNU_SOURCE -W -Wall -o client client.c
	gcc -g -std=c11 -D_GNU_SOURCE -W -Wall -o server server.c

clean:
	rm -f client server

