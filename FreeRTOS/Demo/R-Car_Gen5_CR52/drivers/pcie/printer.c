
/******************************************************************************/
/* How to use mprintf                                                         */
/* "test"     : %s    = test                                                  */
/* 'a'        : %c    = a                                                     */
/* 0x00001234 : %08x  = 00001234                                              */
/* 0x00000012 : %04x  = 0012                                                  */
/* 0x00000001 : %02x  = 01                                                    */
/* 0x12345678 : %x    = 12345678                                              */
/* 0x12345678 : %d    = 305419896                                             */
/* ----- special function %0*b -----                                          */
/* 0x00000000 : %01b  = 0                                                     */
/* 0x00000001 : %01b  = 1                                                     */
/* 0x12345678 : %04b  = 1000                                                  */
/* 0x12345678 : %08b  = 01111000                                              */
/* 0x12345678 : %012b = 011001111000                                          */
/* 0x12345678 : %016b = 0101011001111000                                      */
/* 0x12345678 : %024b = 001101000101011001111000                              */
/* 0x12345678 : %028b = 0010001101000101011001111000                          */
/* 0x12345678 : %032b = 00010010001101000101011001111000                      */
/******************************************************************************/


/******************************************************************************/
/* INCLUDE FILE                                                               */
/******************************************************************************/
#include <stdarg.h>

#define AArch64 1
#define SYSTEM_DEF_X5H 1

#include "drivers/pcie/common.h"
#include "drivers/pcie/stdfunc.h"

/******************************************************************************/
/* INTERNAL FUNCTION PROTOTYPE                                                */
/******************************************************************************/
static char monWrite( char *str, uint64_t count );
static void i2x( uint64_t n , char s[] );
#ifdef AArch64
static void i2u( uint64_t n , char s[] );
#endif
static void i2a( int n , char s[] );
static void i2b( uint64_t, char*);
static void Reverse( char s[] );

////////////////////////////////////////////////////////////////////////////////
// INTERNAL FUNCTION                                                          //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// FUNCTION : mprintf(const char *format ,... )                               //
// ARGUMENT : %02/04/08xXdDuUsScC                                             //
//						x or X : hex                                          //
//						d or D : decimal                                      //
//						u or U : unsigned int                                 //
//						s or S : string                                       //
//						c or C : 1 character                                  //
//			  %01/02/04/08/12/16/20/24/28/30/32bB                             //
//						b or B : binary                                       //
// RETURN   : none                                                            //
// OUTLINE  : Mini printf                                                     //
////////////////////////////////////////////////////////////////////////////////
void mprintf(const char *format,...)
{
	va_list ap;
	uint8_t*	puint8Format = (uint8_t*)format;
	uint8_t		uint8Sdigit[34];
	uint8_t*	puint8Str;

	va_start( ap, format );

	while (*puint8Format != '\0')
	{
		if (*puint8Format == '%')
		{
			puint8Format++;
			switch ( *puint8Format )
			{
				case '0':
				puint8Format++;
				switch ( *puint8Format )
				{
					case '1':
						puint8Format++;
						switch (*puint8Format)
						{
							case 'b':case 'B':		// %b01
								i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit+31, 1 );
								break;
							case '2':
								puint8Format++;
								switch (*puint8Format)
								{
									case 'b':case 'B':		// %b012
										i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
										monWrite( (uint8_t*)uint8Sdigit+20, 12 );
										break;
									default:
										monWrite( (uint8_t*)puint8Format, 1 );
										break;
								}
								break;
							case '6':
								puint8Format++;
								switch (*puint8Format)
								{
									case 'b':case 'B':		// %b016
										i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
										monWrite( (uint8_t*)uint8Sdigit+16, 16 );
										break;
									case 'x':case 'X':
										i2x( va_arg(ap,uint64_t), (char*)uint8Sdigit );
										monWrite( (uint8_t*)uint8Sdigit, 16 );
										break;
									default:
										monWrite( (uint8_t*)puint8Format, 1 );
										break;
								}
								break;
						}
						break;
					case '2':
						puint8Format++;
						switch (*puint8Format)
						{
							case '0':
								puint8Format++;
								switch (*puint8Format)
								{
									case 'b':case 'B':		// %b020
										i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
										monWrite( (uint8_t*)uint8Sdigit+12, 20 );
										break;
									default:
										monWrite( (uint8_t*)puint8Format, 1 );
								}
								break;
							case '4':
								puint8Format++;
								switch (*puint8Format)
								{
									case 'b':case 'B':		// %b024
										i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
										monWrite( (uint8_t*)uint8Sdigit+8, 24 );
										break;
									default:
										monWrite( (uint8_t*)puint8Format, 1 );
										break;
								}
								break;
							case '8':
								puint8Format++;
								switch (*puint8Format)
								{
									case 'b':case 'B':		// %b028
										i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
										monWrite( (uint8_t*)uint8Sdigit+4, 28 );
										break;
									default:
										monWrite( (uint8_t*)puint8Format, 1 );
								}
								break;
							case 'x':case 'X':
								i2x( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit+14, 2 );
								break;
#ifdef AArch64
							case 'u':case 'U':
								i2u( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit+6, 2 );
								break;
#endif
							case 'b':case 'B':		// %b02
								i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit+30, 2 );
								break;
							default:
								monWrite( (uint8_t*)puint8Format, 1 );
								break;
						}
						break;
					case '3':
						puint8Format++;
						switch (*puint8Format)
						{
							case '2':
							puint8Format++;
							switch (*puint8Format)
							{
								case 'b':case 'B':		// %b032
									i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
									monWrite( (uint8_t*)uint8Sdigit, 32 );
									break;
								default:
									monWrite( (uint8_t*)puint8Format, 1 );
									break;
							}
							break;
						}
						break;
					case '4':
						puint8Format++;
						switch ( *puint8Format )
						{
							case 'x':case 'X':
								i2x( va_arg( ap,uintptr_t ), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit+12, 4 );
								break;
#ifdef AArch64
							case 'u':case 'U':
								i2u( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit+4, 4 );
								break;
#endif
							case 'd':case 'D':
								i2a( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit+4, 4 );
								break;
							case 'b':case 'B':
								i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit+28, 4 );
								break;
							default:
								monWrite( (uint8_t*)puint8Format, 1 );
								break;
						}
						break;
					case '8':
						puint8Format++;
						switch ( *puint8Format )
						{
							case 'x':case 'X':
								i2x( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit+8, 8 );
								break;
#ifdef AArch64
							case 'u':case 'U':
								i2u( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit, 8 );
								break;
#endif
							case 'b':case 'B':
								i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
								monWrite( (uint8_t*)uint8Sdigit+24, 8 );
								break;
							default:
								monWrite( (uint8_t*)puint8Format, 1 );
								break;
						}
						break;
					default:
						monWrite( (uint8_t*)puint8Format, 1 );
						break;
					}
					break;
				case 'x':case 'X':
					i2x( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
					puint8Str = uint8Sdigit;
					while( (*puint8Str) == '0' )
					{
						puint8Str++;
					}
					if( *puint8Str == '\0' )
					{
						puint8Str--;
					}
					monWrite( (uint8_t*)puint8Str, strlen( (char*)puint8Str) );
					break;
#ifdef AArch64
				case 'u':case 'U':
					i2u( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
					puint8Str = uint8Sdigit;
					while ( (*puint8Str) == '0' )
					{
						puint8Str++;
					}
					if( *puint8Str == '\0' )
					{
						puint8Str--;
					}
					monWrite( (uint8_t*)puint8Str, strlen( (char*)puint8Str) );
					break;
#endif
				case 'd':case 'D':
					i2a( va_arg(ap,int), (char*)uint8Sdigit );
					monWrite( (uint8_t*)uint8Sdigit, strlen( (char*)uint8Sdigit) );
					break;
				case 's':case 'S':
					puint8Str = (uint8_t*)va_arg( ap,char* );
					monWrite( (uint8_t*)puint8Str, strlen( (char*)puint8Str ) );
					break;
				case 'c':case 'C':
					uint8Sdigit[0] = va_arg( ap, int );
					monWrite( (uint8_t*)&uint8Sdigit[0], 1 );
					break;
				case 'b':case 'B':
					i2b( va_arg(ap,uintptr_t), (char*)uint8Sdigit );
					puint8Str = uint8Sdigit;
					while( (*puint8Str) == '0' )
					{
						puint8Str++;
					}
					if( *puint8Str == '\0' )
					{
						puint8Str--;
					}
					monWrite( (uint8_t*)puint8Str, strlen( (char*)puint8Str) );
					break;
				default:
					monWrite( (uint8_t*)puint8Format, 1 );
					break;
			}
			puint8Format++;
		}
		else{
			monWrite( (uint8_t*)puint8Format, 1 );
			puint8Format++;
		}
	}
	va_end(ap);

	return;
}

////////////////////////////////////////////////////////////////////////////////
// INTERNAL FUNCTION                                                          //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function : monWrite                                                        //
// Parameter: char *str, uint64_t count                                       //
// Return   : number of characters                                            //
// Outline  : Output characters                                               //
////////////////////////////////////////////////////////////////////////////////
static char monWrite( char *str, uint64_t count )
{
	if ( count > 0 )
	{
		while( count > 0 )
		{
			if( *str=='\n' ){
				//Todo:
				//
				//PutChar(0x0D);			// Send "CR"
				//PutChar(0x0A);			// Send "LF"
                                console_putc(0x0D);                        // Send "CR"
                                console_putc(0x0A);                        // Send "LF
				str++;
				count--;
			}else{
				console_putc( *str );
				str++;
				count--;
			}
		}
		return count;
	}
	else
	{
		return 0xFF;
	}
}


////////////////////////////////////////////////////////////////////////////////
// Function : i2x                                                             //
// Parameter: uint64_t n ,char s[]                                            //
// Return   : none                                                            //
// Outline  : decimal -> hex                                                  //
////////////////////////////////////////////////////////////////////////////////
static void i2x( uint64_t n ,char s[] )
{
	int	i = 0;
	int	a;

	do {
		if ((a = (n % 16)) < 10) {
			s[i++] = a + '0';
		}
		else {
			s[i++] = a + '7';
		}
		n /= 16;
	}
	while(i < 16);
	s[i] = '\0';
	Reverse(s);
}

#ifdef AArch64
////////////////////////////////////////////////////////////////////////////////
// Function : i2u                                                             //
// Parameter: uint64_t n ,char s[]                                            //
// Return   : none                                                            //
// Outline  : uint64_t -> string                                              //
////////////////////////////////////////////////////////////////////////////////
static void i2u( uint64_t n , char s[] )
{
	int	i = 0;

	do {
		s[i++] = (n % 10) + '0';
		n /= 10;
	}
	while(i < 16);
	s[i] = '\0';
	Reverse(s);
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Function : i2a                                                             //
// Parameter: int n ,char s[]                                                 //
// Return   : none                                                            //
// Outline  : int -> string                                                   //
////////////////////////////////////////////////////////////////////////////////
static void i2a( int n ,char s[] )
{
	int	i ,sign;

	if ((sign = n) < 0) {
		n = -n;
	}
	i = 0;
	do {
		s[i++] = (n % 10) + '0';
	}
	while((n /= 10) > 0);
	if (sign < 0) {
		s[i++] = '-';
	}
	s[i] = '\0';
	Reverse(s);
}

////////////////////////////////////////////////////////////////////////////////
// FUNCTION : i2b( uint64_t n , char s[] )                                    //
// ARGUMENT : uint32_t n ,char s[]                                            //
// RETURN   : static void                                                     //
// OUTLINE  : binary -> string                                                //
//            No debug-log output                                             //
////////////////////////////////////////////////////////////////////////////////
static void i2b( uint64_t n , char s[] )
{
	uint64_t i;

	for( i=0; i<32; i++ ){
		if( (n>>i)&1 ) s[i] = '1';
		else		   s[i] = '0';
	}
	s[i] = '\0';
	Reverse(s);
}


////////////////////////////////////////////////////////////////////////////////
// Function : Reverse                                                         //
// Parameter: char s[]                                                        //
// Return   : none                                                            //
// Outline  : reverse string                                                  //
////////////////////////////////////////////////////////////////////////////////
static void Reverse( char s[] )
{
	int	c,i,j;

	for ( i = 0, j = strlen(s)-1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
