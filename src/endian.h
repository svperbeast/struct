/*
 * endian.h
 *
 *  Created on: 2011. 5. 2.
 *      Author: Wonseok
 */

#ifndef ENDIAN_H_
#define ENDIAN_H_

enum endian {
	STRUCT_ENDIAN_NOT_SET,
	STRUCT_ENDIAN_BIG,
	STRUCT_ENDIAN_LITTLE
};

extern int get_endian(void);

#endif /* ENDIAN_H_ */
