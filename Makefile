CC = gcc
OBJECTS = shell.o

shell: $(OBJECTS)
	$(CC) -g $(OBJECTS) -o shell

shell.o: shell.c $(HEADERS)
	$(CC) -c shell.c

clean:
	-rm -f $(OBJECTS)
	-rm -f shell
