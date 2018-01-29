//conversion function
//Returns i, j values of any given address

#include "Addr_Conversion.h"

int* Addr_Conversion(char boardposition)
{
	int param_ij[2];

	//setting all i values (rows)
	if (0x0 <= boardposition <= 0x7)		//0->7 i = 0 (1st row)
	{
		param_ij[0] = 0; //i value
	}
	else if (0x8 <= boardposition <= 0xA)	//8->15 i= 1 (2nd row)
	{
		param_ij[0] = 1; //i value
	}
	else if (0xB <= boardposition <= 0x17)	//16->23 i = 2 (3rd row)
	{
		param_ij[0] = 2; //i value
	}
	else if (0x18 <= boardposition <= 0x1F) //24->31 i = 3 (4th row)
	{
		param_ij[0] = 3; //i value
	}
	else if (0x20 <= boardposition <= 0x27) //32->39 i = 4 (5th row)
	{
		param_ij[0] = 4; //i value
	}
	else if (0x28 <= boardposition <= 0x2F) //40->47 i = 5 (6th row)
	{
		param_ij[0] = 5; //i value
	}
	else if (0x30 <= boardposition <= 0x37) //48->55 i = 6 (7th row)
	{
		param_ij[0] = 6; //i value
	}
	else if (0x3A <= boardposition <= 0x3F) //56->63 i = 7 (8th row)
	{
		param_ij[0] = 7; //i vlaue
	}
	else
	{
		param_ij[0] = 0; //default to 1st row
	}

	//setting j values (columns)
	//using boardposition = (8*i) + j --> how board is referenced
	param_ij[1] = boardposition - (param_ij[0] * 8); //setting j value

	return param_ij;
}