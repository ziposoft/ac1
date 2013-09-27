/*________________________________________________________________________

z_utility.c

________________________________________________________________________*/

#include "zipo.h"
#include "z_utility.h"

//TODO make Unicode


U16 z_byte_swap16(U16 x)
{
	union {
		unsigned char	c[2];
		U16 i;
	} wc, wc1 ;

	wc.i = x;	/* assign */

	wc1.c[0] = wc.c[1];	/*swap */
	wc1.c[1] = wc.c[0];

	return(wc1.i);	/* return the swapped result */
}
U32 z_byte_swap32(U32 x)
{
	union {
		unsigned char	c[4];
		unsigned int i;
	} wc, wc1 ;

	wc.i = x;	/* assign */

	wc1.c[0] = wc.c[3];	/*swap */
	wc1.c[1] = wc.c[2];
	wc1.c[2] = wc.c[1];
	wc1.c[3] = wc.c[0];

	return(wc1.i);	/* return the swapped result */
}

int z_ipower(int val,int power)
{
	int i=1;
	while(power--)
	{
		i=i*val;
	}
	return i;
}




