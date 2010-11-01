


void sleep(int time) {
	int i, j;
	int a = 0;
	int b = 3;
	for(i=0; i<0x00005000; i++) { // emu
	//for(i=0; i<0x00033333; i++) { // pc
		for(j=0; j<time; j++)
		{
			a += b;
			b += a;
		}
		b += a;
	}
	a += b;
}
