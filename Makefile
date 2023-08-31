INC += -I/MinGW/include/ncurses
LIB += -I/MinGW/lib -lncurses

snake: main.c
	gcc $(INC) -std=c99 main.c -o snake $(LIB)
