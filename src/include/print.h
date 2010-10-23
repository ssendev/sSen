// header for printing stuff

#ifndef __PRINT_H
#define __PRINT_H

int initScreen(void);

void wrhexe(int);

void setattr(char, char);
#define BLACK	0
#define DBLUE	1
#define DGREEN	2
#define DCYAN	3
#define DRED	4
#define DMAGENTA 5
#define ORANGE	6
#define GREY	7
#define DGREY	8
#define BLUE	9
#define GREEN	10
#define CYAN	11
#define RED	12
#define MAGENTA 13
#define YELLOW	14
#define WHITE	15

char getattr(void);

int setpos(char, char);

typedef struct curpos
{
	char x;
	char y;
} curPos;
curPos getpos();

void clrscr();

void wrch(char);

int wrstr(char *);

#endif // __PRINT_H
