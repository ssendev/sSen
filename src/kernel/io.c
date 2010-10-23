
char inb(unsigned short port) {
	char data;
	__asm__ ("inb %1, %0" : "=a" (data) : "dN" (port));
	return data;
}

void outb(unsigned short port, char data) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

unsigned short inw(unsigned short port) {
	short data;
	__asm__ ("inw %1, %0" : "=a" (data) : "dN" (port));
	return data;
}

void outw(unsigned short port, unsigned short data) {
	__asm__ __volatile__ ("outw %1, %0" : : "dN" (port), "a" (data));
}
