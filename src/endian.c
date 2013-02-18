/*
 * endian.c
 *
 *  Created on: 2011. 5. 2.
 *      Author: Wonseok
 */

#include "endian.h"

int get_endian(void)
{
	int i = 0x00000001;
	if (((char *)&i)[0]) {
		return STRUCT_ENDIAN_LITTLE;
	} else {
		return STRUCT_ENDIAN_BIG;
	}
}
