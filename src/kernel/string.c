

#include <stddef.h>

size_t strlen(const char *s) {
	char *sBeg = (char *) s;
	while (*s != 0)
	{
		s++;
	}

	return (size_t) (s - sBeg);
}


int strcmp(const char *s1, const char *s2)
{
	while (*s1 != 0 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return *s1 - *s2;
}


int strncmp(const char *s1, const char *s2, size_t n)
{
	if (n-- == 0)
		return 0;
	while (*s1 != 0 && *s1 == *s2 && 0< n--)
	{
		s1++;
		s2++;
	}
	return *s1 - *s2;
}


char *strcpy(char *s1, const char *s2)
{
	char *rs=s1;
	*s1 = *s2;
	while (*s2 != 0)
	{
		s1++;
		s2++;
		*s1 = *s2;
	}
	return rs;
}


char *strchr(const char *s, int c)
{
	char *S = (char *) s;
	char C = c;
	do
	{
		if (*S == C)
			return S;
		S++;
	}
	while ( *S != 0 );
	return NULL;
}


char *strncpy(char *s1, const char *s2, size_t n)
{
	char *rs=s1;
	while (*s2 != 0 && 0< n)
	{
		*s1 = *s2;
		s1++;
		s2++;
		n--;
	}
	while ( 0< n-- )
	{
		*s1 = 0;
		s1++;
	}
	return rs;
}


void *memchr(const void *s, int c, size_t n)
{
	unsigned char *S = (unsigned char *) s;
	unsigned char C = c;
	while ( 0< n-- )
	{
		if (*S == C)
			return S;
		S++;
	}
	return NULL;
}


void *memcpy(void *s1, const void *s2, size_t n)
{
	dword *S1 = s1;
	dword *S2 = (dword *) s2;
	size_t N = n/4;
	size_t nByte = n%4;
	while ( 0< N-- )
	{
		*S1 = *S2;
		S1++;
		S2++;
	}
	byte *sByte1 = (byte *) S1;
	byte *sByte2 = (byte *) S2;
	while ( 0< nByte-- )
	{
		*sByte1 = *sByte2;
		sByte1++;
		sByte2++;
	}
	return s1;
}


int memcmp(const void *s1, const void *s2, size_t n)
{
	if (n-- == 0)
		return 0;
	char *S1 = (char *) s1;
	char *S2 = (char *) s2;

	while (*S1 == *S2 && 0< n--)
	{
		S1++;
		S2++;
	}
	return *S1 - *S2;
}


void *memmove_fixme(void *s1, const void *s2, size_t n)
{
	if (n-- == 0)
		return 0;

	if (s1 > s2)
	{
		
	} else {

	}
	return s1;
}



