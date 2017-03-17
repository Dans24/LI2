CFLAGS=-Wall
FICHEIROS=cgi.h estado.c estado.h exemplo.c Makefile

install: exemplo
	sudo cp exemplo /usr/lib/cgi-bin
	touch install

exemplo: exemplo.o estado.o
	cc -o exemplo exemplo.o estado.o

exemplo.zip: $(FICHEIROS)
	zip -9 exemplo.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o exemplo Doxyfile latex html install

estado.o: estado.c estado.h
exemplo.o: exemplo.c cgi.h estado.h
