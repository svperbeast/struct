/*
 * struct_test.cpp
 *
 *  Created on: 2011. 5. 6.
 *      Author: wonseok choi
 */

#include "struct.h"
#include "gtest/gtest.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <limits>
#include <math.h>

namespace {

class Struct:public ::testing::Test {
protected:
  Struct() {}
  virtual ~Struct() {}

  virtual void SetUp()
  {
    memset(buf, 0, sizeof(buf));
  }

  virtual void TearDown() {}

  unsigned char buf[BUFSIZ];
  static const char sc;
  static const unsigned char uc;
};

const char Struct::sc = -42;
const unsigned char Struct::uc = 42;

TEST_F(Struct, memcpyCompatibilityValid)
{
	int16_t i = 0x1234;
	char mbuf[sizeof(int16_t)];

	struct_pack(buf, "h", i);
	memcpy(mbuf, &i, sizeof(int16_t));

	EXPECT_EQ(buf[0], mbuf[0]);
	EXPECT_EQ(buf[1], mbuf[1]);
}

TEST_F(Struct, CalcsizeValid)
{
	char strbuf[32];

	EXPECT_TRUE(sizeof(char) == struct_calcsize("b"));
	EXPECT_TRUE(sizeof(unsigned char) == struct_calcsize("B"));
	EXPECT_TRUE(sizeof(int16_t) == struct_calcsize("h"));
	EXPECT_TRUE(sizeof(uint16_t) == struct_calcsize("H"));
	EXPECT_TRUE(sizeof(int32_t) == struct_calcsize("l"));
	EXPECT_TRUE(sizeof(uint32_t) == struct_calcsize("L"));
	EXPECT_TRUE(sizeof(int32_t) == struct_calcsize("i"));
	EXPECT_TRUE(sizeof(uint32_t) == struct_calcsize("I"));
	EXPECT_TRUE(sizeof(int64_t) == struct_calcsize("q"));
	EXPECT_TRUE(sizeof(uint64_t) == struct_calcsize("Q"));
	EXPECT_TRUE(sizeof(float) == struct_calcsize("f"));
	EXPECT_TRUE(sizeof(double) == struct_calcsize("d"));
	EXPECT_TRUE(sizeof(strbuf) == struct_calcsize("32s"));
	EXPECT_TRUE(sizeof(strbuf) == struct_calcsize("32p"));
	EXPECT_TRUE(sizeof(char) == struct_calcsize("x"));
	EXPECT_TRUE(10 == struct_calcsize("v"));
	EXPECT_TRUE(10 == struct_calcsize("V"));
}

TEST_F(Struct, PadBytePackUnpackingValid1)
{
	char o1, o2;
	struct_pack(buf, "2x2b", sc, sc);
	struct_unpack(buf, "2x2b", &o1, &o2);
	EXPECT_EQ(sc, o1);
	EXPECT_EQ(sc, o2);
}

TEST_F(Struct, PadBytePackUnpackingValid2)
{
	char o1, o2;
	struct_pack(buf, "xbxb", sc, sc);
	struct_unpack(buf, "xbxb", &o1, &o2);
	EXPECT_EQ(sc, o1);
	EXPECT_EQ(sc, o2);
}

TEST_F(Struct, Signed1bytePackUnpackingValid)
{
	char o;
	struct_pack(buf, "b", sc);
	struct_unpack(buf, "b", &o);
	EXPECT_EQ(sc, o);
}

TEST_F(Struct, SignedMultiBytePackUnpackingValid)
{
	char o1, o2, o3, o4;
	struct_pack(buf, "4b", sc, sc, sc, sc);
	struct_unpack(buf, "4b", &o1, &o2, &o3, &o4);
	EXPECT_EQ(sc, o1);
	EXPECT_EQ(sc, o2);
	EXPECT_EQ(sc, o3);
	EXPECT_EQ(sc, o4);
}

TEST_F(Struct, Signed1BytePackUnpackingWithOffsetValid)
{
	char o;
	struct_pack_into(1, buf, "b", sc);
	struct_unpack_from(1, buf, "b", &o);
	EXPECT_EQ(sc, o);
}

TEST_F(Struct, SignedMultiBytePackUnpackingWithOffsetValid)
{
	char o1, o2, o3, o4;
	struct_pack_into(1, buf, "4b", sc, sc, sc, sc);
	struct_unpack_from(1, buf, "4b", &o1, &o2, &o3, &o4);
	EXPECT_EQ(sc, o1);
	EXPECT_EQ(sc, o2);
	EXPECT_EQ(sc, o3);
	EXPECT_EQ(sc, o4);
}

TEST_F(Struct, SingleBytePackUnpackingValid)
{
	unsigned char o;
	struct_pack(buf, "B", uc);
	struct_unpack(buf, "B", &o);
	EXPECT_EQ(uc, o);
}

TEST_F(Struct, MultiBytePackUnpackingValid)
{
	unsigned char o1, o2, o3, o4;
	struct_pack(buf, "4B", uc, uc, uc, uc);
	struct_unpack(buf, "4B", &o1, &o2, &o3, &o4);
	EXPECT_EQ(uc, o1);
	EXPECT_EQ(uc, o2);
	EXPECT_EQ(uc, o3);
	EXPECT_EQ(uc, o4);
}

TEST_F(Struct, SingleBytePackUnpackingWithOffsetValid)
{
	unsigned char o;
	struct_pack_into(1, buf, "B", uc);
	struct_unpack_from(1, buf, "B", &o);
	EXPECT_EQ(uc, o);
}

TEST_F(Struct, MultiBytePackUnpackingWithOffsetValid)
{
	unsigned char o1, o2, o3, o4;
	struct_pack_into(1, buf, "4B", uc, uc, uc, uc);
	struct_unpack_from(1, buf, "4B", &o1, &o2, &o3, &o4);
	EXPECT_EQ(uc, o1);
	EXPECT_EQ(uc, o2);
	EXPECT_EQ(uc, o3);
	EXPECT_EQ(uc, o4);
}

TEST_F(Struct, int16_tPackUnpackingValid)
{
	int16_t i = 0x1234;
	int16_t o;
	struct_pack(buf, "h", i);
	struct_unpack(buf, "h", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int16_tLimitMinPackUnpackingValid)
{
	int16_t i = std::numeric_limits<int16_t>::min();
	int16_t o;
	struct_pack(buf, "h", i);
	struct_unpack(buf, "h", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int16_tLimitMaxPackUnpackingValid)
{
	int16_t i = std::numeric_limits<int16_t>::max();
	int16_t o;
	struct_pack(buf, "h", i);
	struct_unpack(buf, "h", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int16_tNegPackUnpackingValid)
{
	int16_t i = -1234;
	int16_t o;
	struct_pack(buf, "h", i);
	struct_unpack(buf, "h", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int16_tMultiPackUnpackingValid)
{
	int16_t i = 0x1234;
	int16_t o1, o2;
	struct_pack(buf, "2h", i, i);
	struct_unpack(buf, "2h", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int16_tPackUnpackingWithOffsetValid)
{
	int16_t i = 0x1234;
	int16_t o;
	struct_pack_into(1, buf, "h", i);
	struct_unpack_from(1, buf, "h", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int16_tMultiPackUnpackingWithOffsetValid)
{
	int16_t i = 0x1234;
	int16_t o1, o2;
	struct_pack_into(1, buf, "2h", i, i);
	struct_unpack_from(1, buf, "2h", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint16_tPackUnpackingValid)
{
	uint16_t i = 0x1234;
	uint16_t o;
	struct_pack(buf, "H", i);
	struct_unpack(buf, "H", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint16_tLimitMinPackUnpackingValid)
{
	uint16_t i = std::numeric_limits<uint16_t>::min();
	uint16_t o;
	struct_pack(buf, "H", i);
	struct_unpack(buf, "H", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint16_tLimitMaxPackUnpackingValid)
{
	uint16_t i = std::numeric_limits<uint16_t>::max();
	uint16_t o;
	struct_pack(buf, "H", i);
	struct_unpack(buf, "H", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint16_tMultiPackUnpackingValid)
{
	uint16_t i = 0x1234;
	uint16_t o1, o2;
	struct_pack(buf, "2H", i, i);
	struct_unpack(buf, "2H", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint16_tPackUnpackingWithOffsetValid)
{
	uint16_t i = 0x1234;
	uint16_t o;
	struct_pack_into(1, buf, "H", i);
	struct_unpack_from(1, buf, "H", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint16_tMultiPackUnpackingWithOffsetValid)
{
	uint16_t i = 0x1234;
	uint16_t o1, o2;
	struct_pack_into(1, buf, "2H", i, i);
	struct_unpack_from(1, buf, "2H", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int32_t_i_PackUnpackingValid)
{
	int32_t i = 0x12345678;
	int32_t o;
	struct_pack(buf, "i", i);
	struct_unpack(buf, "i", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_i_LimitMinPackUnpackingValid)
{
	int32_t i = std::numeric_limits<int32_t>::min();
	int32_t o;
	struct_pack(buf, "i", i);
	struct_unpack(buf, "i", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_i_LimitMaxPackUnpackingValid)
{
	int32_t i = std::numeric_limits<int32_t>::max();
	int32_t o;
	struct_pack(buf, "i", i);
	struct_unpack(buf, "i", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_i_NegPackUnpackingValid)
{
	int32_t i = -12345678;
	int32_t o;
	struct_pack(buf, "i", i);
	struct_unpack(buf, "i", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_i_MultiPackUnpackingValid)
{
	int32_t i = 0x12345678;
	int32_t o1, o2;
	struct_pack(buf, "2i", i, i);
	struct_unpack(buf, "2i", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int32_t_i_PackUnpackingWithOffsetValid)
{
	int32_t i = 0x12345678;
	int32_t o;
	struct_pack_into(1, buf, "i", i);
	struct_unpack_from(1, buf, "i", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_i_MultiPackUnpackingWithOffsetValid)
{
	int32_t i = 0x12345678;
	int32_t o1, o2;
	struct_pack_into(1, buf, "2i", i, i);
	struct_unpack_from(1, buf, "2i", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint32_t_I_PackUnpackingValid)
{
	uint32_t i = 0x12345678;
	uint32_t o;
	struct_pack(buf, "I", i);
	struct_unpack(buf, "I", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_I_LimitMinPackUnpackingValid)
{
	uint32_t i = std::numeric_limits<uint32_t>::min();
	uint32_t o;
	struct_pack(buf, "I", i);
	struct_unpack(buf, "I", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_I_LimitMaxPackUnpackingValid)
{
	uint32_t i = std::numeric_limits<uint32_t>::max();
	uint32_t o;
	struct_pack(buf, "I", i);
	struct_unpack(buf, "I", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_I_MultiPackUnpackingValid)
{
	uint32_t i = 0x12345678;
	uint32_t o1, o2;
	struct_pack(buf, "2I", i, i);
	struct_unpack(buf, "2I", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint32_t_I_PackUnpackingWithOffsetValid)
{
	uint32_t i = 0x12345678;
	uint32_t o;
	struct_pack_into(1, buf, "I", i);
	struct_unpack_from(1, buf, "I", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_I_MultiPackUnpackingWithOffsetValid)
{
	uint32_t i = 0x12345678;
	uint32_t o1, o2;
	struct_pack_into(1, buf, "2I", i, i);
	struct_unpack_from(1, buf, "2I", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int32_t_l_PackUnpackingValid)
{
	int32_t i = 0x12345678;
	int32_t o;
	struct_pack(buf, "l", i);
	struct_unpack(buf, "l", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_l_LimitMinPackUnpackingValid)
{
	int32_t i = std::numeric_limits<int32_t>::min();
	int32_t o;
	struct_pack(buf, "l", i);
	struct_unpack(buf, "l", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_l_LimitMaxPackUnpackingValid)
{
	int32_t i = std::numeric_limits<int32_t>::max();
	int32_t o;
	struct_pack(buf, "l", i);
	struct_unpack(buf, "l", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_l_NegPackUnpackingValid)
{
	int32_t i = -12345678;
	int32_t o;
	struct_pack(buf, "l", i);
	struct_unpack(buf, "l", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_l_MultiPackUnpackingValid)
{
	int32_t i = 0x12345678;
	int32_t o1, o2;
	struct_pack(buf, "2l", i, i);
	struct_unpack(buf, "2l", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int32_t_l_PackUnpackingWithOffsetValid)
{
	int32_t i = 0x12345678;
	int32_t o;
	struct_pack_into(1, buf, "l", i);
	struct_unpack_from(1, buf, "l", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_l_MultiPackUnpackingWithOffsetValid)
{
	int32_t i = 0x12345678;
	int32_t o1, o2;
	struct_pack_into(1, buf, "2l", i, i);
	struct_unpack_from(1, buf, "2l", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint32_t_L_PackUnpackingValid)
{
	uint32_t i = 0x12345678;
	uint32_t o;
	struct_pack(buf, "L", i);
	struct_unpack(buf, "L", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_L_LimitMinPackUnpackingValid)
{
	uint32_t i = std::numeric_limits<uint32_t>::min();
	uint32_t o;
	struct_pack(buf, "L", i);
	struct_unpack(buf, "L", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_L_LimitMaxPackUnpackingValid)
{
	uint32_t i = std::numeric_limits<uint32_t>::max();
	uint32_t o;
	struct_pack(buf, "L", i);
	struct_unpack(buf, "L", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_L_MultiPackUnpackingValid)
{
	uint32_t i = 0x12345678;
	uint32_t o1, o2;
	struct_pack(buf, "2L", i, i);
	struct_unpack(buf, "2L", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint32_t_L_PackUnpackingWithOffsetValid)
{
	uint32_t i = 0x12345678;
	uint32_t o;
	struct_pack_into(1, buf, "L", i);
	struct_unpack_from(1, buf, "L", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_L_MultiPackUnpackingWithOffsetValid)
{
	uint32_t i = 0x12345678;
	uint32_t o1, o2;
	struct_pack_into(1, buf, "2L", i, i);
	struct_unpack_from(1, buf, "2L", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int64_tPackUnpackingValid)
{
	int64_t i = 0x1234567887654321LL;
	int64_t o;
	struct_pack(buf, "q", i);
	struct_unpack(buf, "q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int64_t_LimitMinPackUnpackingValid)
{
	int64_t i = std::numeric_limits<int64_t>::min();
	int64_t o;
	struct_pack(buf, "q", i);
	struct_unpack(buf, "q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int64_t_LimitMaxPackUnpackingValid)
{
	int64_t i = std::numeric_limits<int64_t>::max();
	int64_t o;
	struct_pack(buf, "q", i);
	struct_unpack(buf, "q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int64_tNegPackUnpackingValid)
{
	int64_t i = -1234567887654321;
	int64_t o;
	struct_pack(buf, "q", i);
	struct_unpack(buf, "q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int64_tMultiPackUnpackingValid)
{
	int64_t i = 0x1234567887654321LL;
	int64_t o1, o2;
	struct_pack(buf, "2q", i, i);
	struct_unpack(buf, "2q", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int64_tPackUnpackingWithOffsetValid)
{
	int64_t i = 0x1234567887654321LL;
	int64_t o;
	struct_pack_into(1, buf, "q", i);
	struct_unpack_from(1, buf, "q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int64_t_MultiPackUnpackingWithOffsetValid)
{
	int64_t i = 0x1234567887654321LL;
	int64_t o1, o2;
	struct_pack_into(1, buf, "2q", i, i);
	struct_unpack_from(1, buf, "2q", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint64_tPackUnpackingValid)
{
	uint64_t i = 0x1234567887654321LL;
	uint64_t o;
	struct_pack(buf, "Q", i);
	struct_unpack(buf, "Q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint64_t_LimitMinPackUnpackingValid)
{
	uint64_t i = std::numeric_limits<uint64_t>::min();
	uint64_t o;
	struct_pack(buf, "Q", i);
	struct_unpack(buf, "Q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint64_t_LimitMaxPackUnpackingValid)
{
	uint64_t i = std::numeric_limits<uint64_t>::max();
	uint64_t o;
	struct_pack(buf, "Q", i);
	struct_unpack(buf, "Q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint64_tMultiPackUnpackingValid)
{
	uint64_t i = 0x1234567887654321LL;
	uint64_t o1, o2;
	struct_pack(buf, "2Q", i, i);
	struct_unpack(buf, "2Q", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint64_tPackUnpackingWithOffsetValid)
{
	uint64_t i = 0x1234567887654321LL;
	uint64_t o;
	struct_pack_into(1, buf, "Q", i);
	struct_unpack_from(1, buf, "Q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint64_t_MultiPackUnpackingWithOffsetValid)
{
	uint64_t i = 0x1234567887654321LL;
	uint64_t o1, o2;
	struct_pack_into(1, buf, "2Q", i, i);
	struct_unpack_from(1, buf, "2Q", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, floatPackUnpackingValid)
{
	float i = 3.141592;
	float o;
	struct_pack(buf, "f", i);
	struct_unpack(buf, "f", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, floatNegPackUnpackingValid)
{
	float i = -3.141592;
	float o;
	struct_pack(buf, "f", i);
	struct_unpack(buf, "f", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, floatMultiPackUnpackingValid)
{
	float i = 3.141592;
	float o1, o2;
	struct_pack(buf, "2f", i, i);
	struct_unpack(buf, "2f", &o1, &o2);
	EXPECT_DOUBLE_EQ(i, o1);
	EXPECT_DOUBLE_EQ(i, o2);
}

TEST_F(Struct, floatPackUnpackingWithOffsetValid)
{
	float i = 3.141592;
	float o;
	struct_pack_into(1, buf, "f", i);
	struct_unpack_from(1, buf, "f", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, floatMultiPackUnpackingWithOffsetValid)
{
	float i = 3.141592;
	float o1, o2;
	struct_pack_into(1, buf, "2f", i, i);
	struct_unpack_from(1, buf, "2f", &o1, &o2);
	EXPECT_DOUBLE_EQ(i, o1);
	EXPECT_DOUBLE_EQ(i, o2);
}

TEST_F(Struct, doublePackUnpackingValid)
{
	double i = 3.141592;
	double o;
	struct_pack(buf, "d", i);
	struct_unpack(buf, "d", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, doubleNegPackUnpackingValid)
{
	double i = -3.141592;
	double o;
	struct_pack(buf, "d", i);
	struct_unpack(buf, "d", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, doubleMultiPackUnpackingValid)
{
	double i = 3.141592;
	double o1, o2;
	struct_pack(buf, "2d", i, i);
	struct_unpack(buf, "2d", &o1, &o2);
	EXPECT_DOUBLE_EQ(i, o1);
	EXPECT_DOUBLE_EQ(i, o2);
}

TEST_F(Struct, doublePackUnpackingWithOffsetValid)
{
	double i = 3.141592;
	double o;
	struct_pack_into(1, buf, "d", i);
	struct_unpack_from(1, buf, "d", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, doubleMultiPackUnpackingWithOffsetValid)
{
	double i = 3.141592;
	double o1, o2;
	struct_pack_into(1, buf, "2d", i, i);
	struct_unpack_from(1, buf, "2d", &o1, &o2);
	EXPECT_DOUBLE_EQ(i, o1);
	EXPECT_DOUBLE_EQ(i, o2);
}

TEST_F(Struct, StringPackUnpackingValidWithS)
{
	char str[32];
	char o[32];
	char fmt[32];

	memset(str, 0, sizeof(str));
	memset(o, 0, sizeof(o));
	memset(fmt, 0, sizeof(fmt));
	strcpy(str, "test0");
	sprintf(fmt, "%ds", (int)strlen(str));

	struct_pack(buf, fmt, str);
	struct_unpack(buf, fmt, o);
	EXPECT_STREQ(str, o);
}

TEST_F(Struct, StringPackUnpackingValidWithP)
{
	char str[32];
	char o[32];
	char fmt[32];

	memset(str, 0, sizeof(str));
	memset(o, 0, sizeof(o));
	memset(fmt, 0, sizeof(fmt));
	strcpy(str, "test0");
	sprintf(fmt, "%dp", (int)strlen(str));

	struct_pack(buf, fmt, str);
	struct_unpack(buf, fmt, o);
	EXPECT_STREQ(str, o);
}

TEST_F(Struct, StringMultiPackUnpackingValid)
{
	char str1[32];
	char str2[32];
	char o1[32];
	char o2[32];
	char fmt[32];

	memset(str1, 0, sizeof(str1));
	memset(str2, 0, sizeof(str2));
	memset(o1, 0, sizeof(o1));
	memset(o2, 0, sizeof(o2));
	memset(fmt, 0, sizeof(fmt));
	strcpy(str1, "test0");
	strcpy(str2, "test1");
	sprintf(fmt, "%ds%ds", (int)strlen(str1), (int)strlen(str2));

	struct_pack(buf, fmt, str1, str2);
	struct_unpack(buf, fmt, o1, o2);
	EXPECT_STREQ(str1, o1);
	EXPECT_STREQ(str2, o2);
}

TEST_F(Struct, int16_t_big_PackUnpackingValid)
{
	int16_t i = 0x1234;
	int16_t o;
	struct_pack(buf, ">h", i);
	struct_unpack(buf, ">h", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int16_t_big_MultiPackUnpackingValid)
{
	int16_t i = 0x1234;
	int16_t o1, o2;
	struct_pack(buf, ">2h", i, i);
	struct_unpack(buf, ">2h", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int16_t_big_PackUnpackingWithOffsetValid)
{
	int16_t i = 0x1234;
	int16_t o;
	struct_pack_into(1, buf, ">h", i);
	struct_unpack_from(1, buf, ">h", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int16_t_big_MultiPackUnpackingWithOffsetValid)
{
	int16_t i = 0x1234;
	int16_t o1, o2;
	struct_pack_into(1, buf, ">2h", i, i);
	struct_unpack_from(1, buf, ">2h", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint16_t_big_PackUnpackingValid)
{
	uint16_t i = 0x1234;
	uint16_t o;
	struct_pack(buf, ">H", i);
	struct_unpack(buf, ">H", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint16_t_big_MultiPackUnpackingValid)
{
	uint16_t i = 0x1234;
	uint16_t o1, o2;
	struct_pack(buf, ">2H", i, i);
	struct_unpack(buf, ">2H", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint16_t_big_PackUnpackingWithOffsetValid)
{
	uint16_t i = 0x1234;
	uint16_t o;
	struct_pack_into(1, buf, ">H", i);
	struct_unpack_from(1, buf, ">H", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint16_t_big_MultiPackUnpackingWithOffsetValid)
{
	uint16_t i = 0x1234;
	uint16_t o1, o2;
	struct_pack_into(1, buf, ">2H", i, i);
	struct_unpack_from(1, buf, ">2H", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int32_t_i_big_PackUnpackingValid)
{
	int32_t i = 0x12345678;
	int32_t o;
	struct_pack(buf, ">i", i);
	struct_unpack(buf, ">i", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_i_big_MultiPackUnpackingValid)
{
	int32_t i = 0x12345678;
	int32_t o1, o2;
	struct_pack(buf, ">2i", i, i);
	struct_unpack(buf, ">2i", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int32_t_i_big_PackUnpackingWithOffsetValid)
{
	int32_t i = 0x12345678;
	int32_t o;
	struct_pack_into(1, buf, ">i", i);
	struct_unpack_from(1, buf, ">i", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_i_big_MultiPackUnpackingWithOffsetValid)
{
	int32_t i = 0x12345678;
	int32_t o1, o2;
	struct_pack_into(1, buf, ">2i", i, i);
	struct_unpack_from(1, buf, ">2i", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint32_t_I_big_PackUnpackingValid)
{
	uint32_t i = 0x12345678;
	uint32_t o;
	struct_pack(buf, ">I", i);
	struct_unpack(buf, ">I", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_I_big_MultiPackUnpackingValid)
{
	uint32_t i = 0x12345678;
	uint32_t o1, o2;
	struct_pack(buf, ">2I", i, i);
	struct_unpack(buf, ">2I", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint32_t_I_big_PackUnpackingWithOffsetValid)
{
	uint32_t i = 0x12345678;
	uint32_t o;
	struct_pack_into(1, buf, ">I", i);
	struct_unpack_from(1, buf, ">I", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_I_big_MultiPackUnpackingWithOffsetValid)
{
	uint32_t i = 0x12345678;
	uint32_t o1, o2;
	struct_pack_into(1, buf, ">2I", i, i);
	struct_unpack_from(1, buf, ">2I", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int32_t_l_big_PackUnpackingValid)
{
	int32_t i = 0x12345678;
	int32_t o;
	struct_pack(buf, ">l", i);
	struct_unpack(buf, ">l", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_l_big_MultiPackUnpackingValid)
{
	int32_t i = 0x12345678;
	int32_t o1, o2;
	struct_pack(buf, ">2l", i, i);
	struct_unpack(buf, ">2l", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int32_t_l_big_PackUnpackingWithOffsetValid)
{
	int32_t i = 0x12345678;
	int32_t o;
	struct_pack_into(1, buf, ">l", i);
	struct_unpack_from(1, buf, ">l", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int32_t_l_big_MultiPackUnpackingWithOffsetValid)
{
	int32_t i = 0x12345678;
	int32_t o1, o2;
	struct_pack_into(1, buf, ">2l", i, i);
	struct_unpack_from(1, buf, ">2l", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint32_t_L_big_PackUnpackingValid)
{
	uint32_t i = 0x12345678;
	uint32_t o;
	struct_pack(buf, ">L", i);
	struct_unpack(buf, ">L", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_L_big_MultiPackUnpackingValid)
{
	uint32_t i = 0x12345678;
	uint32_t o1, o2;
	struct_pack(buf, ">2L", i, i);
	struct_unpack(buf, ">2L", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint32_t_L_big_PackUnpackingWithOffsetValid)
{
	uint32_t i = 0x12345678;
	uint32_t o;
	struct_pack_into(1, buf, ">L", i);
	struct_unpack_from(1, buf, ">L", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint32_t_L_big_MultiPackUnpackingWithOffsetValid)
{
	uint32_t i = 0x12345678;
	uint32_t o1, o2;
	struct_pack_into(1, buf, ">2L", i, i);
	struct_unpack_from(1, buf, ">2L", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int64_t_big_PackUnpackingValid)
{
	int64_t i = 0x1234567887654321LL;
	int64_t o;
	struct_pack(buf, ">q", i);
	struct_unpack(buf, ">q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int64_t_big_MultiPackUnpackingValid)
{
	int64_t i = 0x1234567887654321LL;
	int64_t o1, o2;
	struct_pack(buf, ">2q", i, i);
	struct_unpack(buf, ">2q", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, int64_t_big_PackUnpackingWithOffsetValid)
{
	int64_t i = 0x1234567887654321LL;
	int64_t o;
	struct_pack_into(1, buf, ">q", i);
	struct_unpack_from(1, buf, ">q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, int64_t_big_MultiPackUnpackingWithOffsetValid)
{
	int64_t i = 0x1234567887654321LL;
	int64_t o1, o2;
	struct_pack_into(1, buf, ">2q", i, i);
	struct_unpack_from(1, buf, ">2q", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint64_t_big_PackUnpackingValid)
{
	uint64_t i = 0x1234567887654321LL;
	uint64_t o;
	struct_pack(buf, ">Q", i);
	struct_unpack(buf, ">Q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint64_t_big_MultiPackUnpackingValid)
{
	uint64_t i = 0x1234567887654321LL;
	uint64_t o1, o2;
	struct_pack(buf, ">2Q", i, i);
	struct_unpack(buf, ">2Q", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, uint64_t_big_PackUnpackingWithOffsetValid)
{
	uint64_t i = 0x1234567887654321LL;
	uint64_t o;
	struct_pack_into(1, buf, ">Q", i);
	struct_unpack_from(1, buf, ">Q", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, uint64_t_big_MultiPackUnpackingWithOffsetValid)
{
	uint64_t i = 0x1234567887654321LL;
	uint64_t o1, o2;
	struct_pack_into(1, buf, ">2Q", i, i);
	struct_unpack_from(1, buf, ">2Q", &o1, &o2);
	EXPECT_EQ(i, o1);
	EXPECT_EQ(i, o2);
}

TEST_F(Struct, float_big_PackUnpackingValid)
{
	float i = 3.141592;
	float o;
	struct_pack(buf, ">f", i);
	struct_unpack(buf, ">f", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, float_big_MultiPackUnpackingValid)
{
	float i = 3.141592;
	float o1, o2;
	struct_pack(buf, ">2f", i, i);
	struct_unpack(buf, ">2f", &o1, &o2);
	EXPECT_DOUBLE_EQ(i, o1);
	EXPECT_DOUBLE_EQ(i, o2);
}

TEST_F(Struct, float_big_PackUnpackingWithOffsetValid)
{
	float i = 3.141592;
	float o;
	struct_pack_into(1, buf, ">f", i);
	struct_unpack_from(1, buf, ">f", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, float_big_MultiPackUnpackingWithOffsetValid)
{
	float i = 3.141592;
	float o1, o2;
	struct_pack_into(1, buf, ">2f", i, i);
	struct_unpack_from(1, buf, ">2f", &o1, &o2);
	EXPECT_DOUBLE_EQ(i, o1);
	EXPECT_DOUBLE_EQ(i, o2);
}

TEST_F(Struct, double_big_PackUnpackingValid)
{
	double i = 3.141592;
	double o;
	struct_pack(buf, ">d", i);
	struct_unpack(buf, ">d", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, double_big_MultiPackUnpackingValid)
{
	double i = 3.141592;
	double o1, o2;
	struct_pack(buf, ">2d", i, i);
	struct_unpack(buf, ">2d", &o1, &o2);
	EXPECT_DOUBLE_EQ(i, o1);
	EXPECT_DOUBLE_EQ(i, o2);
}

TEST_F(Struct, double_big_PackUnpackingWithOffsetValid)
{
	double i = 3.141592;
	double o;
	struct_pack_into(1, buf, ">d", i);
	struct_unpack_from(1, buf, ">d", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, double_big_MultiPackUnpackingWithOffsetValid)
{
	double i = 3.141592;
	double o1, o2;
	struct_pack_into(1, buf, ">2d", i, i);
	struct_unpack_from(1, buf, ">2d", &o1, &o2);
	EXPECT_DOUBLE_EQ(i, o1);
	EXPECT_DOUBLE_EQ(i, o2);
}

TEST_F(Struct, ExamplePacketPackUnpackingValid)
{
	char msg_id = 1;
	short count = 60000;
	char val = 128;
	int32_t data = 0x11223344;
	char omsg_id;
	short ocount;
	char oval;
	int32_t odata;

	struct_pack(buf, "bhbl",
				msg_id,
				count,
				val,
				data);
	struct_unpack(buf, "bhbl",
				  &omsg_id,
				  &ocount,
				  &oval,
				  &odata);

	EXPECT_TRUE(msg_id == omsg_id);
	EXPECT_TRUE(count == ocount);
	EXPECT_TRUE(val == oval);
	EXPECT_TRUE(data == odata);
}

TEST_F(Struct, ExamplePacketPackUnpackingWithOffsetValid)
{
	char msg_id = 1;
	short count = 60000;
	char val = 128;
	int32_t data = 0x11223344;
	char omsg_id;
	short ocount;
	char oval;
	int32_t odata;
	int offset;

	struct_pack(buf, "b", msg_id);
	offset = sizeof(msg_id);

	struct_pack_into(offset, buf, "h", count);
	offset += sizeof(count);

	struct_pack_into(offset, buf, "b", val);
	offset += sizeof(val);

	struct_pack_into(offset, buf, "l", data);

	// -->

	struct_unpack(buf, "b", &omsg_id);
	offset = sizeof(omsg_id);

	struct_unpack_from(offset, buf, "h", &ocount);
	offset += sizeof(ocount);

	struct_unpack_from(offset, buf, "b", &oval);
	offset += sizeof(oval);

	struct_unpack_from(offset, buf, "l", &odata);

	EXPECT_TRUE(msg_id == omsg_id);
	EXPECT_TRUE(count == ocount);
	EXPECT_TRUE(val == oval);
	EXPECT_TRUE(data == odata);
}

TEST_F(Struct, HandleFloatInf)
{
	float i = 1.0 / 0.0;
	float o;
	struct_pack(buf, "f", i);
	struct_unpack(buf, "f", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, HandleFloatNegativeInf)
{
	float i = -1.0 / 0.0;
	float o;
	struct_pack(buf, "f", i);
	struct_unpack(buf, "f", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, HandleFloatInfBigEndian)
{
	float i = 1.0 / 0.0;
	float o;
	struct_pack(buf, "!f", i);
	struct_unpack(buf, "!f", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, HandleFloatNegativeInfBigEndian)
{
	float i = -1.0 / 0.0;
	float o;
	struct_pack(buf, "!f", i);
	struct_unpack(buf, "!f", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, HandleDoubleInf)
{
	double i = 1.0 / 0.0;
	double o;
	struct_pack(buf, "d", i);
	struct_unpack(buf, "d", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, HandleDoubleNegativeInf)
{
	double i = -1.0 / 0.0;
	double o;
	struct_pack(buf, "d", i);
	struct_unpack(buf, "d", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, HandleDoubleInfBigEndian)
{
	double i = 1.0 / 0.0;
	double o;
	struct_pack(buf, "!d", i);
	struct_unpack(buf, "!d", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, HandleDoubleNegativeInfBigEndian)
{
	double i = -1.0 / 0.0;
	double o;
	struct_pack(buf, "!d", i);
	struct_unpack(buf, "!d", &o);
	EXPECT_DOUBLE_EQ(i, o);
}

TEST_F(Struct, HandleFloatNan)
{
	float i = 0.0 / 0.0;
	float o;
	struct_pack(buf, "f", i);
	struct_unpack(buf, "f", &o);
	EXPECT_TRUE(i != o); // NaN != NaN => true
}

TEST_F(Struct, HandleFloatNanBigEndian)
{
	float i = 0.0 / 0.0;
	float o;
	struct_pack(buf, "!f", i);
	struct_unpack(buf, "!f", &o);
	EXPECT_TRUE(i != o); // NaN != NaN => true
}

TEST_F(Struct, HandleDoubleNan)
{
	double i = 0.0 / 0.0;
	double o;
	struct_pack(buf, "d", i);
	struct_unpack(buf, "d", &o);
	EXPECT_TRUE(i != o); // NaN != NaN => true
}

TEST_F(Struct, HandleDoubleNanBigEndian)
{
	double i = 0.0 / 0.0;
	double o;
	struct_pack(buf, "!d", i);
	struct_unpack(buf, "!d", &o);
	EXPECT_TRUE(i != o); // NaN != NaN => true
}

TEST_F(Struct, PackUnpackingStringUsingSameBuffer)
{
	char str[32];
	char fmt[32];

	memset(str, 0, sizeof(str));
	memset(fmt, 0, sizeof(fmt));

	strcpy(str, "test");
	snprintf(fmt, sizeof(fmt), "%ds", (int)strlen(str));
	// checking the result of snprintf() omitted..

	struct_pack(str, fmt, str);
	struct_unpack(str, fmt, str);
	EXPECT_STREQ(str, "test");
}

TEST_F(Struct, varint_PackUnpackingValid)
{
	uint64_t i = 0x1234567887654321LL;
	uint64_t o = 0;
	struct_pack(buf, ">V", i);
	struct_unpack(buf, ">V", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, varint_SignedPackUnpackingValid)
{
	int64_t i = -18014398573270210LL;
	int64_t o;
	struct_pack(buf, ">v", i);
	struct_unpack(buf, ">v", &o);
	EXPECT_EQ(i, o);
}

TEST_F(Struct, varint_MultiPackUnpackingValid)
{
	uint64_t i1 = 7182, i2 = 0x1234567887654321LL;
	uint64_t o1, o2;
	struct_pack(buf, ">2V", i1, i2);
	struct_unpack(buf, ">2V", &o1, &o2);
	EXPECT_EQ(i1, o1);
	EXPECT_EQ(i2, o2);
}

} // namespace

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
