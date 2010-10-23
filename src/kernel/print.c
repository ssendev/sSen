// her'e the functions for printing on screen.
//

#include <print.h>
#include <io.h>

#define lines 25
#define rows 80

const short int *video_base;
short int *address;

int cur_x;
int cur_y;

char attrib;


// initialise variables and set colormode
int initScreen(void) {
	video_base = (short int *) 0xB8000;
	cur_x = 0;
	cur_y = 0;
	attrib = 0x07;

	char colorreg;	
	inb(0x3DA);
	outb(0x3C0, 0x30);
	colorreg = inb(0x3C1);
	colorreg = colorreg &0xF7;	// &0xF7 = color  ::  |0x08 = blink
	outb(0x3C0, colorreg);

	clrscr();

	return 0;
}


// move coursor
void curmov(void) {
	short int pos = cur_y * rows + cur_x;
	outb(0x3D4, 14);
	outb(0x3D5, pos >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, pos);
}


// check if scrolling is needed and scroll
void scroll(void) {
	if(cur_y>=lines) {
		int scrl_lines = cur_y - lines +1;
		int *src = (int *) video_base + scrl_lines * rows/2;
		int *dst = (int *) video_base ;
		int i;
		// move screencontent
		for(i=0; i<lines*rows/2 ; i++) {
			*dst = *src;
			dst++;
			src++;
		}
		// clear last line
		dst = (int *) video_base + lines * rows;
		for(i=0; i<rows/2; i++) {
			*dst = attrib<<24 | ' '<<16 | attrib<<8 | ' ';
			dst++;
		}
		// clear memory after screen			FIXME shouldnt use such an approach
		dst = (int *) video_base + lines * rows/2;
		for(i=0; i<rows/2*scrl_lines ; i++) {
			*dst = attrib<<24 | ' '<<16 | attrib<<8 | ' ';
			dst++;
		}
		cur_y = lines-1;
	}
	curmov();
}


// print a 32 bit hexnumber
void wrhexe(int num) {
	address = (short int *) video_base + cur_y*rows + cur_x +7;
	char table[] = "0123456789ABCDEF";
	int i;
	for(i=0; i<8; i++) {
		*address = table[num & 0xF] | attrib<<8;
		address--;
		num=num>>4;
	}
	cur_x+=8;
	if(cur_x>rows) {
		cur_x-=rows;
		cur_y++;
		//scroll();
	}
}


// set textcolor
void setattr(char bg, char fg) {
	attrib = ( bg<<4 ) | ( fg &0xF );
}

char getattr(void) {
	return attrib;
}

// set the cursor position to X, Y
int setpos(char x, char y) {
	if(x<rows && y<lines) {
		cur_x = x;
		cur_y = y;
		curmov();
		return 0;
	} else
		return 1;
}


curPos getpos() {
	curPos cur_pos;
	cur_pos.x=cur_x;
	cur_pos.y=cur_y;
	return cur_pos;
}

// fills the screen with spaces
void clrscr(void) {
	cur_y = 0;
	cur_x = 0;
	int i;
	int *addr = (int *) video_base;
	for(i=0; i<rows*lines/2; i++) {
		*addr = attrib<<24 | ' '<<16 | attrib<<8 | ' ';
		addr++;
	}
	curmov();
}


// intern function from wrch and wrstr
void wrchI(char c) {
	if(c >=' ') {	// is printable character?
		*address = c | attrib<<8;
		address++;
	} else if(c == 0x0A) {	// \n  is newline?
		address += rows - (int) (address - video_base) % rows;
	} else if(c == 0x08) {	// \b  is backspace? 
		if((int) (address - video_base) % rows > 0)
			address--;
		*address = ' ' | attrib<<8;
	} else if(c == 0x0D) {	// \r  is carriage return?
		address -= (int) (address - video_base) % rows;
	} else {
		// print inverted pipe to indicate special character
		*address = c | ( attrib>>4 | attrib<<4 )<<8 ;
		address++;
	}
}


// prints one character
void wrch(char c) {
	address = (short int *) video_base + cur_y*rows + cur_x;
	wrchI(c);

	// calculate new cursor position
	cur_y = (int) (address - video_base) / rows;
	cur_x = (int) (address - video_base) % rows;
	scroll();
}


// prints a 0 terminated string
int wrstr(char *str) {
	address = (short int *) video_base + cur_y*rows + cur_x;

	char *strStart = str;
	while( *str != 0 ) {
		wrchI(*str);
		str++;
	}

	// calculate new cursor position
	cur_y = (int) (address - video_base) / rows;
	cur_x = (int) (address - video_base) % rows;
	scroll();

	return str - strStart;
}

