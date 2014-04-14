#include "struct.h"
#include "endian.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// refer to
// http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#serialization
// - Beej's Guide to Network Programming
//
// macros for packing floats and doubles:
#define PACK_IEEE754_16(f) (pack_ieee754((f), 16, 5))
#define PACK_IEEE754_32(f) (pack_ieee754((f), 32, 8))
#define PACK_IEEE754_64(f) (pack_ieee754((f), 64, 11))
#define UNPACK_IEEE754_16(i) (unpack_ieee754((i), 16, 5))
#define UNPACK_IEEE754_32(i) (unpack_ieee754((i), 32, 8))
#define UNPACK_IEEE754_64(i) (unpack_ieee754((i), 64, 11))

#define CHECK_PREREQUISITE() do { \
	if (STRUCT_ENDIAN_NOT_SET == myendian) { \
		struct_init(); \
	} \
} while (0)

#define CHECK_REPETITION(n, i, b) do { \
	if (0 < (i)) { \
		errno = 0; \
		(n) = strtol((b), NULL, 10); \
		if (errno != 0) { \
			break; \
		} \
	} else { \
		(n) = 1; \
	} \
	(i) = 0; \
	memset((b), 0, sizeof((b))); \
} while (0)

static int myendian = STRUCT_ENDIAN_NOT_SET;

static void struct_init(void)
{
	myendian = get_endian();
}

static unsigned long long int pack_ieee754(long double f,
        unsigned bits, unsigned expbits)
{
    long double fnorm;
    int shift;
    long long sign;
    long long exp;
    long long significand;
    unsigned int significandbits = bits - expbits - 1; // -1 for sign bit

    if (f == 0.0) {
        return 0; // get this special case out of the way
    }

    // check sign and begin normalization
    if (f < 0) {
        sign = 1;
        fnorm = -f;
    } else {
        sign = 0;
        fnorm = f;
    }

    // get the normalized form of f and track the exponent
    shift = 0;
    while (fnorm >= 2.0) {
        fnorm /= 2.0;
        shift++;
    }
    while (fnorm < 1.0) {
        fnorm *= 2.0;
        shift--;
    }
    fnorm = fnorm - 1.0;

    // calculate the binary form (non-float) of the significand data
    significand = fnorm * ((1LL << significandbits) + 0.5f);

    // get the biased exponent
    exp = shift + ((1 << (expbits - 1)) - 1); // shift + bias

    // return the final answer
    return (sign << (bits - 1)) | (exp << (bits - expbits - 1)) | significand;
}

static long double unpack_ieee754(unsigned long long int i,
        unsigned bits, unsigned expbits)
{
    long double result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (i == 0) {
        return 0.0;
    }

    // pull the significand
    result = (i & ((1LL << significandbits) - 1)); // mask
    result /= (1LL << significandbits); // convert back to float
    result += 1.0f; // add the one back on

    // deal with the exponent
    bias = (1 << (expbits - 1)) - 1;
    shift = ((i >> significandbits) & ((1LL << expbits) - 1)) - bias;
    while (shift > 0) {
        result *= 2.0;
        shift--;
    }
    while (shift < 0) {
        result /= 2.0;
        shift++;
    }

    // sign it
    result *= ((i >> (bits - 1)) & 1) ? -1.0 : 1.0;

    return result;
}

static void pack_int16_t(unsigned char **bp, uint16_t val, int endian)
{
	if (endian == myendian) {
		*((*bp)++) = val;
		*((*bp)++) = val >> 8;
	} else {
		*((*bp)++) = val >> 8;
		*((*bp)++) = val;
	}
}

static void pack_int32_t(unsigned char **bp, uint32_t val, int endian)
{
	if (endian == myendian) {
		*((*bp)++) = val;
		*((*bp)++) = val >> 8;
		*((*bp)++) = val >> 16;
		*((*bp)++) = val >> 24;
	} else {
		*((*bp)++) = val >> 24;
		*((*bp)++) = val >> 16;
		*((*bp)++) = val >> 8;
		*((*bp)++) = val;
	}
}

static void pack_int64_t(unsigned char **bp, uint64_t val, int endian)
{
	if (endian == myendian) {
		*((*bp)++) = val;
		*((*bp)++) = val >> 8;
		*((*bp)++) = val >> 16;
		*((*bp)++) = val >> 24;
		*((*bp)++) = val >> 32;
		*((*bp)++) = val >> 40;
		*((*bp)++) = val >> 48;
		*((*bp)++) = val >> 56;
	} else {
		*((*bp)++) = val >> 56;
		*((*bp)++) = val >> 48;
		*((*bp)++) = val >> 40;
		*((*bp)++) = val >> 32;
		*((*bp)++) = val >> 24;
		*((*bp)++) = val >> 16;
		*((*bp)++) = val >> 8;
		*((*bp)++) = val;
	}
}

static void pack_float(unsigned char **bp, float val, int endian)
{
    uint64_t ieee754_encoded_val = PACK_IEEE754_32(val);
    pack_int32_t(bp, ieee754_encoded_val, endian);
}

static void pack_double(unsigned char **bp, double val, int endian)
{
    uint64_t ieee754_encoded_val = PACK_IEEE754_64(val);
    pack_int64_t(bp, ieee754_encoded_val, endian);
}

static void unpack_int16_t(unsigned char **bp, int16_t *dst, int endian)
{
    uint16_t val;
	if (endian == myendian) {
		val = *((*bp)++);
		val |= (uint16_t)(*((*bp)++)) << 8;
	} else {
		val = (uint16_t)(*((*bp)++)) << 8;
		val |= *((*bp)++);
	}
    if (val <= 0x7fffU) {
        *dst = val;
    } else {
        *dst = -1 - (int16_t)(0xffffU - val);
    }
}

static void unpack_uint16_t(unsigned char **bp, uint16_t *dst, int endian)
{
	if (endian == myendian) {
		*dst = *((*bp)++);
		*dst |= (uint16_t)(*((*bp)++)) << 8;
	} else {
		*dst = (uint16_t)(*((*bp)++)) << 8;
		*dst |= *((*bp)++);
	}
}

static void unpack_int32_t(unsigned char **bp, int32_t *dst, int endian)
{
    uint32_t val;
	if (endian == myendian) {
		val = *((*bp)++);
		val |= (uint32_t)(*((*bp)++)) << 8;
		val |= (uint32_t)(*((*bp)++)) << 16;
		val |= (uint32_t)(*((*bp)++)) << 24;
	} else {
		val = *((*bp)++) << 24;
		val |= (uint32_t)(*((*bp)++)) << 16;
		val |= (uint32_t)(*((*bp)++)) << 8;
		val |= (uint32_t)(*((*bp)++));
	}
    if (val <= 0x7fffffffU) {
        *dst = val;
    } else {
        *dst = -1 - (int32_t)(0xffffffffU - val);
    }
}

static void unpack_uint32_t(unsigned char **bp, uint32_t *dst, int endian)
{
	if (endian == myendian) {
		*dst = *((*bp)++);
		*dst |= (uint32_t)(*((*bp)++)) << 8;
		*dst |= (uint32_t)(*((*bp)++)) << 16;
		*dst |= (uint32_t)(*((*bp)++)) << 24;
	} else {
		*dst = *((*bp)++) << 24;
		*dst |= (uint32_t)(*((*bp)++)) << 16;
		*dst |= (uint32_t)(*((*bp)++)) << 8;
		*dst |= (uint32_t)(*((*bp)++));
	}
}

static void unpack_int64_t(unsigned char **bp, int64_t *dst, int endian)
{
    uint64_t val;
	if (endian == myendian) {
		val = *((*bp)++);
		val |= (uint64_t)(*((*bp)++)) << 8;
		val |= (uint64_t)(*((*bp)++)) << 16;
		val |= (uint64_t)(*((*bp)++)) << 24;
		val |= (uint64_t)(*((*bp)++)) << 32;
		val |= (uint64_t)(*((*bp)++)) << 40;
		val |= (uint64_t)(*((*bp)++)) << 48;
		val |= (uint64_t)(*((*bp)++)) << 56;
	} else {
		val = (uint64_t)(*((*bp)++)) << 56;
		val |= (uint64_t)(*((*bp)++)) << 48;
		val |= (uint64_t)(*((*bp)++)) << 40;
		val |= (uint64_t)(*((*bp)++)) << 32;
		val |= (uint64_t)(*((*bp)++)) << 24;
		val |= (uint64_t)(*((*bp)++)) << 16;
		val |= (uint64_t)(*((*bp)++)) << 8;
		val |= *((*bp)++);
	}
    if (val <= 0x7fffffffffffffffU) {
        *dst = val;
    } else {
        *dst = -1 -(int64_t)(0xffffffffffffffffU - val);
    }
}

static void unpack_uint64_t(unsigned char **bp, uint64_t *dst, int endian)
{
	if (endian == myendian) {
		*dst = *((*bp)++);
		*dst |= (uint64_t)(*((*bp)++)) << 8;
		*dst |= (uint64_t)(*((*bp)++)) << 16;
		*dst |= (uint64_t)(*((*bp)++)) << 24;
		*dst |= (uint64_t)(*((*bp)++)) << 32;
		*dst |= (uint64_t)(*((*bp)++)) << 40;
		*dst |= (uint64_t)(*((*bp)++)) << 48;
		*dst |= (uint64_t)(*((*bp)++)) << 56;
	} else {
		*dst = (uint64_t)(*((*bp)++)) << 56;
		*dst |= (uint64_t)(*((*bp)++)) << 48;
		*dst |= (uint64_t)(*((*bp)++)) << 40;
		*dst |= (uint64_t)(*((*bp)++)) << 32;
		*dst |= (uint64_t)(*((*bp)++)) << 24;
		*dst |= (uint64_t)(*((*bp)++)) << 16;
		*dst |= (uint64_t)(*((*bp)++)) << 8;
		*dst |= *((*bp)++);
	}
}

static void unpack_float(unsigned char **bp, float *dst, int endian)
{
    uint32_t ieee754_encoded_val = 0;
    unpack_uint32_t(bp, &ieee754_encoded_val, endian);
    *dst = UNPACK_IEEE754_32(ieee754_encoded_val);
}

static void unpack_double(unsigned char **bp, double *dst, int endian)
{
    uint64_t ieee754_encoded_val = 0;
    unpack_uint64_t(bp, &ieee754_encoded_val, endian);
    *dst = UNPACK_IEEE754_64(ieee754_encoded_val);
}

static int pack_va_list(unsigned char *buf, int offset, const char *fmt,
						  va_list args)
{
	const char *p;
	unsigned char *bp;
	int *ep = &myendian;
	int endian;
	char num_buf[32];
	int num_buf_idx;
	int num;
	int i;

	char b;
	unsigned char B;
	int16_t h;
	uint16_t H;
	int32_t l;
	uint32_t L;
	int64_t q;
	uint64_t Q;
	float f;
	double d;
	char *s;

	CHECK_PREREQUISITE();

	memset(num_buf, 0, sizeof(num_buf));
	num_buf_idx = 0;

	/*
	 * 'char' and 'short' values, they must be extracted as 'int's,
	 * because C promotes 'char' and 'short' arguments to 'int' when they are
	 * represented by an ellipsis ... parameter.
	 */

	bp = buf + offset;
	for (p = fmt; *p != '\0'; p++) {
		switch (*p) {
		case '=': /* native */
			ep = &myendian;
			break;
		case '<': /* little-endian */
			endian = STRUCT_ENDIAN_LITTLE;
			ep = &endian;
			break;
		case '>': /* big-endian */
			endian = STRUCT_ENDIAN_BIG;
			ep = &endian;
			break;
		case '!': /* network (= big-endian) */
			endian = STRUCT_ENDIAN_BIG;
			ep = &endian;
			break;
		case 'b':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				b = va_arg(args, int);
				*bp++ = b;
			}
			break;
		case 'B':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				B = va_arg(args, unsigned int);
				*bp++ = B;
			}
			break;
		case 'h':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				h = va_arg(args, int);
				pack_int16_t(&bp, h, *ep);
			}
			break;
		case 'H':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				H = va_arg(args, int);
				pack_int16_t(&bp, H, *ep);
			}
			break;
		case 'i': /* fall through */
		case 'l':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				l = va_arg(args, int32_t);
				pack_int32_t(&bp, l, *ep);
			}
			break;
		case 'I': /* fall through */
		case 'L':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				L = va_arg(args, uint32_t);
				pack_int32_t(&bp, L, *ep);
			}
			break;
		case 'q':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				q = va_arg(args, int64_t);
				pack_int64_t(&bp, q, *ep);
			}
			break;
		case 'Q':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				Q = va_arg(args, uint64_t);
				pack_int64_t(&bp, Q, *ep);
			}
			break;
		case 'f':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				f = va_arg(args, double);
				pack_float(&bp, f, *ep);
			}
			break;
		case 'd':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				d = va_arg(args, double);
				pack_double(&bp, d, *ep);
			}
			break;
		case 's':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			s = va_arg(args, char*);
			for (i = 0; i < num; i++) {
				*bp++ = s[i];
			}
			break;
		default:
			if (isdigit((int)*p)) {
				num_buf[num_buf_idx++] = *p;
			} else {
				return -1;
			}
		}
	}
	return (bp - buf);
}

static int unpack_va_list(unsigned char *buf, int offset, const char *fmt,
					 va_list args)
{
	const char *p;
	unsigned char *bp;
	int *ep = &myendian;
	int endian;
	char num_buf[32];
	int num_buf_idx;
	int num;
	int i;

	char *b;
	unsigned char *B;
	int16_t *h;
	uint16_t *H;
	int32_t *l;
	uint32_t *L;
	int64_t *q;
	uint64_t *Q;
	float *f;
	double *d;
	char *s;

	CHECK_PREREQUISITE();

	memset(num_buf, 0, sizeof(num_buf));
	num_buf_idx = 0;

	bp = buf + offset;
	for (p = fmt; *p != '\0'; p++) {
		switch (*p) {
		case '=': /* native */
			ep = &myendian;
			break;
		case '<': /* little-endian */
			endian = STRUCT_ENDIAN_LITTLE;
			ep = &endian;
			break;
		case '>': /* big-endian */
			endian = STRUCT_ENDIAN_BIG;
			ep = &endian;
			break;
		case '!': /* network (= big-endian) */
			endian = STRUCT_ENDIAN_BIG;
			ep = &endian;
			break;
		case 'b':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				b = va_arg(args, char*);
				*b = *bp++;
			}
			break;
		case 'B':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				B = va_arg(args, unsigned char*);
				*B = *bp++;
			}
			break;
		case 'h':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				h = va_arg(args, int16_t*);
				unpack_int16_t(&bp, h, *ep);
			}
			break;
		case 'H':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				H = va_arg(args, uint16_t*);
				unpack_uint16_t(&bp, H, *ep);
			}
			break;
		case 'i': /* fall through */
		case 'l':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				l = va_arg(args, int32_t*);
				unpack_int32_t(&bp, l, *ep);
			}
			break;
		case 'I': /* fall through */
		case 'L':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				L = va_arg(args, uint32_t*);
				unpack_uint32_t(&bp, L, *ep);
			}
			break;
		case 'q':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				q = va_arg(args, int64_t*);
				unpack_int64_t(&bp, q, *ep);
			}
			break;
		case 'Q':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				Q = va_arg(args, uint64_t*);
				unpack_uint64_t(&bp, Q, *ep);
			}
			break;
		case 'f':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				f = va_arg(args, float*);
				unpack_float(&bp, f, *ep);
			}
			break;
		case 'd':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			for (i = 0; i < num; i++) {
				d = va_arg(args, double*);
				unpack_double(&bp, d, *ep);
			}
			break;
		case 's':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			s = va_arg(args, char*);
			for (i = 0; i < num; i++) {
				s[i] = *bp++;
			}
			break;
		default:
			if (isdigit((int)*p)) {
				num_buf[num_buf_idx++] = *p;
			} else {
				return -1;
			}
		}
	}
	return (bp - buf);
}

/*
 * EXPORT
 *
 * preifx: struct_
 *
 */
int struct_pack(unsigned char *buf, const char *fmt, ...)
{
	va_list args;
	int packed_len = 0;

	va_start(args, fmt);
	packed_len = pack_va_list(buf, 0, fmt, args);
	va_end(args);

	return packed_len;
}

int struct_pack_into(int offset, unsigned char *buf, const char *fmt, ...)
{
	va_list args;
	int packed_len = 0;

	va_start(args, fmt);
	packed_len = pack_va_list(buf, offset, fmt, args);
	va_end(args);

	return packed_len;
}

int struct_unpack(unsigned char *buf, const char *fmt, ...)
{
	va_list args;
	int unpacked_len = 0;

	va_start(args, fmt);
	unpacked_len = unpack_va_list(buf, 0, fmt, args);
	va_end(args);

	return unpacked_len;
}

int struct_unpack_from(int offset, unsigned char *buf, const char *fmt, ...)
{
	va_list args;
	int unpacked_len = 0;

	va_start(args, fmt);
	unpacked_len = unpack_va_list(buf, offset, fmt, args);
	va_end(args);

	return unpacked_len;
}

int struct_calcsize(const char *fmt)
{
	int ret;
	const char *p;
	char num_buf[32];
	int num_buf_idx;
	int num;

	CHECK_PREREQUISITE();

	ret = 0;
	memset(num_buf, 0, sizeof(num_buf));
	num_buf_idx = 0;

	for (p = fmt; *p != '\0'; p++) {
		switch (*p) {
		case '=': /* fall through */
		case '<': /* fall through */
		case '>': /* fall through */
		case '!': /* ignore endian characters */
			break;
		case 'b':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(char));
			break;
		case 'B':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(unsigned char));
			break;
		case 'h':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(int16_t));
			break;
		case 'H':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(uint16_t));
			break;
		case 'i': /* fall through */
		case 'l':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(int32_t));
			break;
		case 'I': /* fall through */
		case 'L':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(uint32_t));
			break;
		case 'q':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(int64_t));
			break;
		case 'Q':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(uint64_t));
			break;
		case 'f':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(float));
			break;
		case 'd':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(double));
			break;
		case 's':
			CHECK_REPETITION(num, num_buf_idx, num_buf);
			ret += (num * sizeof(char));
			break;
		default:
			if (isdigit((int)*p)) {
				num_buf[num_buf_idx++] = *p;
			} else {
				return -1;
			}
		}
	}
	return ret;
}
