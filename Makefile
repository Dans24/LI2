CFLAGS=-Wall

install: jogo
	sudo cp jogo /usr/lib/cgi-bin/
	sudo cp pacman.png /var/www/html/images
	touch jogo

jogo: main.o jogo.o
	cc -o jogo main.o jogo.o
