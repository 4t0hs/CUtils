#include "gtest/gtest.h"
#include <Bitset/Bitset.h>
#include <iostream>
#include <bitset>
#include <vector>
#include <bitset>

// NOLINTBEGIN
class BitsetTest :public::testing::Test {

protected:
	static constexpr std::uint64_t numBits = 128;
	Bitset_t bitset;
	std::bitset<numBits> stlBitset;
	void SetUp() override {
		Bitset_Init(&bitset, 128);
	}
	void TearDown() override {
		Bitset_Destroy(&bitset);
	}
	std::vector<std::uint64_t> Get() {
		return std::vector<std::uint64_t>(bitset.words, bitset.words + bitset.capacity);
	}
};

TEST_F(BitsetTest, Initialize) {
	Bitset_t bs;
	Bitset_Init(&bs, 8);
	EXPECT_EQ(1, bs.capacity);
	Bitset_Destroy(&bs);

	Bitset_Init(&bs, 100);
	EXPECT_EQ(2, bs.capacity);
	Bitset_Destroy(&bs);
}

TEST_F(BitsetTest, Or) {
	std::uint64_t input1 = 1;
	Bitset_Or(&bitset, input1);
	auto output = Get();
	EXPECT_EQ(input1, output[0]);
	EXPECT_EQ(0, output[1]);

	std::uint64_t input2 = 0xf1;
	Bitset_Or(&bitset, input2);
	output = Get();
	EXPECT_EQ(input1 | input2, output[0]);
	EXPECT_EQ(0, output[1]);
}

TEST_F(BitsetTest, And) {
	std::uint64_t input{ 0x0f };
	Bitset_And(&bitset, input);
	std::vector<std::uint64_t> output1 = Get();
	EXPECT_EQ(0, output1[0]);
	EXPECT_EQ(0, output1[1]);

	Bitset_Or(&bitset, 0xff);
	Bitset_And(&bitset, input);
	std::vector<std::uint64_t> output2 = Get();
	EXPECT_EQ(input, output2[0]);
	EXPECT_EQ(0, output2[1]);
}

TEST_F(BitsetTest, ExclusiveOr) {
	std::uint64_t input{ 0x0f };
	Bitset_Or(&bitset, input);
	Bitset_ExclusiveOr(&bitset, input);
	std::vector<std::uint64_t> output = Get();
	EXPECT_EQ(0, output[0]);
	EXPECT_EQ(0, output[1]);
}

TEST_F(BitsetTest, Clear) {
	Bitset_Or(&bitset, 0xff);
	Bitset_Clear(&bitset);
	std::vector<std::uint64_t> output = Get();
	EXPECT_EQ(0, output[0]);
	EXPECT_EQ(0, output[1]);
}

TEST_F(BitsetTest, Set) {
	Bitset_Set(&bitset, 0);
	Bitset_Set(&bitset, 64);
	std::vector<std::uint64_t> output1 = Get();
	EXPECT_EQ(1, output1[0]);
	EXPECT_EQ(1, output1[1]);

	Bitset_Set(&bitset, numBits + 1);
	std::vector<std::uint64_t> output2 = Get();
	EXPECT_EQ(1, output2[0]);
	EXPECT_EQ(1, output2[1]);
}

TEST_F(BitsetTest, Reset) {
	Bitset_Set(&bitset, 0);
	Bitset_Set(&bitset, 64);

	Bitset_Reset(&bitset, numBits + 1);
	std::vector<std::uint64_t> output1 = Get();
	EXPECT_EQ(1, output1[0]);
	EXPECT_EQ(1, output1[1]);
	
	Bitset_Reset(&bitset, 0);
	Bitset_Reset(&bitset, 64);
	std::vector<std::uint64_t> output2 = Get();
	EXPECT_EQ(0, output2[0]);
	EXPECT_EQ(0, output2[1]);
}

TEST_F(BitsetTest, Flip) {
	uint64_t input{ 0x0f };
	Bitset_Or(&bitset, input);
	Bitset_Flip(&bitset);
	std::vector<std::uint64_t> output = Get();
	EXPECT_EQ(~input, output[0]);
	EXPECT_EQ(~0UL, output[1]);
}

TEST_F(BitsetTest, Count) {
	Bitset_Set(&bitset, 0);
	Bitset_Set(&bitset, 64);
	EXPECT_EQ(2, Bitset_Count(&bitset));
}

TEST_F(BitsetTest, Test) {
	Bitset_Set(&bitset, 0);
	Bitset_Set(&bitset, 64);
	EXPECT_EQ(true, Bitset_Test(&bitset, 0));
	EXPECT_EQ(false, Bitset_Test(&bitset, 1));
	EXPECT_EQ(true, Bitset_Test(&bitset, 64));
	EXPECT_EQ(false, Bitset_Test(&bitset, 65));

	EXPECT_EQ(false, Bitset_Test(&bitset, numBits + 1));
}

TEST_F(BitsetTest, Any) {
	EXPECT_EQ(false, Bitset_Any(&bitset));
	Bitset_Set(&bitset, 0);
	EXPECT_EQ(true, Bitset_Any(&bitset));
}

TEST_F(BitsetTest, All) {
	Bitset_Or(&bitset, 0xf);
	EXPECT_EQ(false, Bitset_All(&bitset));
	Bitset_Clear(&bitset);
	Bitset_Flip(&bitset);
	EXPECT_EQ(true, Bitset_All(&bitset));
}

TEST_F(BitsetTest, None) {
	EXPECT_EQ(true, Bitset_None(&bitset));
	Bitset_Set(&bitset, 0);
	EXPECT_EQ(false, Bitset_None(&bitset));
}

TEST_F(BitsetTest, LeftShift) {
	std::uint64_t input{ 0x0f };
	Bitset_Or(&bitset, input);

	Bitset_LeftShift(&bitset, 64);
	std::vector<std::uint64_t> output1 = Get();
	EXPECT_EQ(0, output1[0]);
	EXPECT_EQ(input, output1[1]);

	Bitset_LeftShift(&bitset, 4);
	std::vector<std::uint64_t> output2 = Get();
	EXPECT_EQ(0, output2[0]);
	EXPECT_EQ(input << 4, output2[1]);

	Bitset_LeftShift(&bitset, 200);
	std::vector<std::uint64_t> output3 = Get();
	EXPECT_EQ(0, output3[0]);
	EXPECT_EQ(0, output3[1]);
}

TEST_F(BitsetTest, RightShift) {
	std::uint64_t input{ 0x0f };
	Bitset_Or(&bitset, input);
	Bitset_LeftShift(&bitset, 64);

	Bitset_RightShift(&bitset, 64);
	std::vector<std::uint64_t> output1 = Get();
	EXPECT_EQ(input, output1[0]);
	EXPECT_EQ(0, output1[1]);

	Bitset_LeftShift(&bitset, 64);
	Bitset_RightShift(&bitset, 2);
	std::vector<std::uint64_t> output2 = Get();
	EXPECT_EQ(input << 62, output2[0]);
	EXPECT_EQ(input >> 2, output2[1]);

	Bitset_RightShift(&bitset, 200);
	std::vector<std::uint64_t> output3 = Get();
	EXPECT_EQ(0, output3[0]);
	EXPECT_EQ(0, output3[1]);
}

TEST_F(BitsetTest, ToString) {
	Bitset_Set(&bitset, 0);
	Bitset_Set(&bitset, 64);
	stlBitset.set(0);
	stlBitset.set(64);
	EXPECT_STREQ(stlBitset.to_string().c_str(), Bitset_ToString(&bitset));
}

TEST_F(BitsetTest, SegmentationFault) {
	Bitset_Init(nullptr, 0);
	Bitset_Destroy(nullptr);

	Bitset_RightShift(nullptr, 1);

	Bitset_LeftShift(nullptr, 1);

	Bitset_ToString(nullptr);

	Bitset_Clear(nullptr);

	Bitset_And(nullptr, 0);

	Bitset_Or(nullptr, 0);

	Bitset_ExclusiveOr(nullptr, 0);

	Bitset_Set(nullptr, 0);

	Bitset_Reset(nullptr, 0);

	Bitset_Flip(nullptr);

	EXPECT_EQ(0, Bitset_Count(nullptr));

	EXPECT_EQ(false, Bitset_Test(nullptr, 0));

	EXPECT_EQ(false, Bitset_Any(nullptr));

	EXPECT_EQ(false, Bitset_All(nullptr));

	EXPECT_EQ(false, Bitset_None(nullptr));
}


// NOLINTEND
