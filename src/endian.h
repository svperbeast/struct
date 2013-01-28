/*
 * endian.h
 *
 *  Created on: 2011. 5. 2.
 *      Author: Wonseok
 */

#ifndef ENDIAN_H_
#define ENDIAN_H_

enum endian {
	NOT_SET,
	BIG_ENDIAN,
	LITTLE_ENDIAN
};

extern int get_endian(void);

#endif /* ENDIAN_H_ */
