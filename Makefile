# -*- Makefile -*-

#Syntax:
#Target: Dependencies
#	Action

all: MusicGenerator

MusicGenerator: musicGenerator.o noteGenerator.o
	gcc musicGenerator.o noteGenerator.o -o MusicGenerator

noteGenerator.o: noteGenerator.c
	gcc -c noteGenerator.c

musicGenerator.o: musicGenerator.c
	gcc -c musicGenerator.c