

#include <stdio.h>
#include <stdlib.h>


int main(int c,char* argv[])
{
	int x=0;
	int ipmbAEnable=0;
	if(c>1)
		sscanf(argv[1],"%d",&x);
	if(c>2)
		sscanf(argv[2],"%d",&ipmbAEnable);

	switch (x)
	{
	case 0:
		printf("case 0\n");
		break;
	case 1:
		printf("case 1\n");
		if (!(ipmbAEnable)) 
		{
		case 2:
			/* set in case we have a i2c failure */
			printf("case 2\n");
		} 
		else 
		{
			case 3:
				printf("case 3\n");
		}
		break;
	case 4:
		printf("case 4\n");
		break;
	default:
		printf("case default\n");
		


	}
	printf("exiting...\n");
	return 0;
}
#if 0
	switch(ipmbAor) 
	{
	case IPMB0_SET_OVERRIDE:
		/* overriding IPMBA by command */
		ipmbAoverridestate = IPMB0A_OVERRIDE;
		break;
	case IPMB0_SET_LC:
		if (!(ipmbAEnable)) {
	case IPMB0_SET_LCFAIL:
		/* set in case we have a i2c failure */
		ipmbAoverridestate = IPMB0A_LCFAILED;
		} else {
	case IPMB0_SET_LCOK:
		ipmbAoverridestate = IPMB0A_LCOK;
		}
		break;
	}

#endif






