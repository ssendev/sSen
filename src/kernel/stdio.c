// stdio.h - standard buffered input/output
#include <stdarg.h>
#include <print.h>
#include <string.h>

#define MAXNUMLENGTH 11

char *itoa(char *s, int i)
{
	int sign;
	if (i < 0)
	{
		i = -i;
		sign = 1;
	} else {
		sign = 0;
	}

	*s += MAXNUMLENGTH;
	*s = 0;
	while (i > 0)
	{
		s--;
		*s = (i%10) | 0x30;
		i = i/10;
	}

	if (sign != 0) {
		s--;
		*s = '-';
	}

	return s;
}


char *utoa(char *s, unsigned int i)
{
	*s += MAXNUMLENGTH;
	*s = 0;

	while (i > 0)
	{
		s--;
		*s = (i%10) | 0x30;
		i = i/10;
	}

	return s;
}


char *hextab = "0123456789ABCDEF";

char *xtoa(char *s, unsigned int i)
{
	*s += MAXNUMLENGTH;
	*s = 0;
	while (i>0)
	{
		s--;
		*s = hextab[i%0x10];
		i = i / 0x10;
	}

	return s;
}



// supports currently only
// %%, %s, %i, %u, %x
// %-s
// %+i, % i
// %10s
// and variations
int kprintf(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	int count = 0;
	char buff[MAXNUMLENGTH+1];
	char *c = (char *) format;
	while (*c != 0)
	{
		if (*c == '%')
		{
			c++;

			// parse the options
			char justify;
			if (*c == '-') {
				justify = '-';
				c++;
			} else {
				justify = '+';
			}

			char sign;
			if (*c == '+') {
				sign = '+';
				c++;
			} else {
				sign = '-';
			}
			if (*c == ' ')
			{
				sign = ' ';
				c++;
			}

			int minLength = 0;
			if (*c >= '0' && *c <= '9') {
				while (*c >= '0' && *c <= '9')
				{
					minLength *= 10;
					minLength += *c - 0x30;
					c++;
				}
			} else {
				minLength = 1;
			}

			// generate the outputstring
			char *pos;
			int length;
			if (*c == '%') {
				*buff = '%';
				*(buff+1) = 0;
				pos = buff;
				length = 1;
			} else if (*c == 's') {
				pos = (va_arg(ap, char *));
				length = strlen(pos);
			} else if (*c == 'i' || *c == 'd') {
				pos = itoa(buff, va_arg(ap, int));
				if (*pos != '-') {
					if (sign == '+')
					{
						*--pos = '+';
					} else if (sign == ' ') {
						*--pos = ' ';
					}
				}
				length = buff - pos;
			} else if (*c == 'u') {
				pos = utoa(buff, va_arg(ap, unsigned int));
				length = buff - pos;
			} else if (*c == 'x') {
				pos = xtoa(buff, va_arg(ap, unsigned int));
				length = buff - pos;
			} else {
				*buff = 0;
				pos = buff;
				length = 0;
			}
			if ( (*c == 'u' || *c == 'x' || *c == 'i' || *c == 'd') && length == 0 && minLength > 0)
			{
				*buff = '0';
				*(buff+1) = 0;
				length = 1;
			}

			// print justified results (add trailing/leading blanks)
			if (justify == '+')
			{
				while (minLength > length )
				{
					wrch(' ');
					length++;
					count++;
				}
				count += wrstr(pos);
			} else {
				count += wrstr(pos);
				while (minLength > length )
				{
					wrch(' ');
					length++;
					count++;
				}
			}
		} else {
			wrch(*c);
			count++;
		}

		c++;
	}

	va_end(ap);

	return count;
}



