/*
 * struct.h
 *
 *  Created on: 2011. 5. 2.
 *      Author: Wonseok
 *
 *
 * Interpret strings as packed binary data
 *
 *
 * Table 1. Byte order
 *  ----------------------------------
 *  Character | Byte order
 *  ----------+-----------------------
 *   =        | native
 *  ----------+-----------------------
 *   <        | little-endian
 *  ----------+-----------------------
 *   >        | big-endian
 *  ----------+-----------------------
 *   !        | network (= big-endian)
 *  ----------------------------------
 *
 * Table 2. Format characters
 *  -------------------------------------------
 *  Format | C/C++ Type         | Standard size
 *  -------+--------------------+--------------
 *   b     | char               | 1
 *  -------+--------------------+--------------
 *   B     | unsigned char      | 1
 *  -------+--------------------+--------------
 *   h     | short              | 2
 *  -------+--------------------+--------------
 *   H     | unsigned short     | 2
 *  -------+--------------------+--------------
 *   i     | int                | 4
 *  -------+--------------------+--------------
 *   I     | unsigned int       | 4
 *  -------+--------------------+--------------
 *   l     | long               | 4
 *  -------+--------------------+--------------
 *   L     | unsigned long      | 4
 *  -------+--------------------+--------------
 *   q     | long long          | 8
 *  -------+--------------------+--------------
 *   Q     | unsigned long long | 8
 *  -------+--------------------+--------------
 *   f     | float              | 4
 *  -------+--------------------+--------------
 *   d     | double             | 8
 *  -------+--------------------+--------------
 *   s     | char[]             |
 *  -------------------------------------------
 *
 * A format character may be preceded by an integral repeat count.
 * For example, the format string '4h' means exactly the same as 'hhhh'.
 *
 * For the 's' format character, the count is interpreted as the size of the string,
 * not a repeat count like for the other format characters.
 * For example, '10s' means a single 10-byte string.
 *
 * Example 1. pack/unpack int type value.
 *
 * char buf[BUFSIZ] = {0, };
 * int val = 0x12345678;
 * int oval;
 *
 * struct_pack(buf, "i", val);
 * struct_unpack(buf, "i", &oval);
 *
 * Example 2. pack/unpack a string.
 *
 * char buf[BUFSIZ] = {0, );
 * char str[32] = {'\0', };
 * char fmt[32] = {'\0', };
 * char ostr[32] = {'\0', };
 *
 * strcpy(str, "test");
 * sprintf(fmt, "%ds", strlen(str));
 *
 * struct_pack(buf, fmt, str);
 * struct_unpack(buf, fmt, ostr);
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

extern int struct_pack(unsigned char *buf, const char *fmt, ...);
extern int struct_pack_into(int offset, unsigned char *buf, const char *fmt, ...);
extern int struct_unpack(unsigned char *buf, const char *fmt, ...);
extern int struct_unpack_from(int offset, unsigned char *buf, const char *fmt, ...);
extern int struct_calcsize(const char *fmt);

#ifdef __cplusplus
}
#endif
