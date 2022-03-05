run:	utils.o	macroFile.o	main2.o
	gcc	-ansi	-Wall	-pedantic	-o	run 
utils.o:	utils.c	utils.h
	gcc	-c	-ansi	-Wall	-pedantic	utils.c	-o	utils.o
macroFile.o:	macro.c	utils.h
	gcc	-c	-ansi	-Wall	-pedantic	macroFile.c	-o	macro.o
main2.o:	main2.c	utils.h
	gcc	-c	-ansi	-Wall	-pedantic	main2.c	-o	main.o
