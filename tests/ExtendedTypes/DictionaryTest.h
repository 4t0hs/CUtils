#pragma once

extern "C" {
#include "ExtendedTypes/Dictionary.h"
}
#include <string>
#include <cstring>
#include <list>
#include "gtest/gtest.h"

void DictionaryTest_Normally(void);

class DictionaryTest : public ::testing::Test {
protected:
	size_t capacity = 3;
	size_t maxKeySize = 8;
	size_t maxObjectSize = 4;
	std::string keys[3] = { "apple", "banana", "orange" };
	int objects[3] = { 120, 200, 150 };
	Dictionary_t dict;
	virtual void SetUp() {
		Dictionary_Init(&dict, capacity, maxKeySize, maxObjectSize);
	}
	virtual void TearDown() {
		Dictionary_Destroy(&dict);
	}
public:
	void AddAll() {
		for (size_t i = 0; i < capacity; i++) {
			ASSERT_EQ(DICTIONARY_ADDED, Dictionary_Add(&dict, keys[i].c_str(), (void *)&objects[i], 4));
		}
	}
};

TEST_F(DictionaryTest, Normally) {
	AddAll();
	for (size_t i = 0; i < capacity; i++) {
		const DictionaryObject_t *obj = Dictionary_Find(&dict, (char *)keys[i].c_str());
		EXPECT_GE(maxObjectSize, obj->size);
		EXPECT_EQ(objects[i], *(int *)obj->buffer);
	}
}

TEST(DictionaryTestAdditional, Normally2) {
	Dictionary_t dict;
	const int cap = 2;
	std::string keys[cap] = { "apple", "peach" };
	int objects[cap] = { 200, 300 };
	Dictionary_Init(&dict, cap, 6, 4);
	for (int i = 0; i < cap; i++) {
		ASSERT_EQ(DICTIONARY_ADDED, Dictionary_Add(&dict, keys[i].c_str(), &objects[i], 4));
	}
	for (int i = 0; i < cap; i++) {
		const DictionaryObject_t *o = Dictionary_Find(&dict, (char *)keys[i].c_str());
		ASSERT_FALSE(o == NULL);
		EXPECT_EQ(4, o->size);
		EXPECT_EQ(objects[i], *(int *)o->buffer);
	}
	Dictionary_Destroy(&dict);
	
}

TEST(DictionaryTestAdditional, Normally3) {
	std::string keys[2] = { "apple", "banana" };
	std::string objects[2] = { "Produced in China", "Produced in India" };
	Dictionary_t dict;
	Dictionary_Init(&dict, 2, 32, 32);
	for (int i = 0; i < 2; i++) {
		ASSERT_EQ(DICTIONARY_ADDED, Dictionary_Add(&dict, keys[i].c_str(), (void *)objects[i].c_str(), objects[i].length()));
	}
	for (int i = 0; i < 2; i++) {
		const DictionaryObject_t *o = Dictionary_Find(&dict, (char *)keys[i].c_str());
		ASSERT_FALSE(o == NULL);
		EXPECT_EQ(objects[i].length(), o->size);
		EXPECT_EQ(0, std::memcmp((void *)objects[i].c_str(), o->buffer, objects[i].length()));
	}
	Dictionary_Destroy(&dict);
}

TEST_F(DictionaryTest, KeyNotFound) {
	AddAll();
	std::string key = "peach";
	EXPECT_EQ(NULL, Dictionary_Find(&dict, (char *)key.c_str()));
}

TEST_F(DictionaryTest, CapacityOver) {
	AddAll();
	int object = 300;
	EXPECT_EQ(DICTIONARY_FULL, Dictionary_Add(&dict, "peach", &object, 4));
}

TEST_F(DictionaryTest, KeySizeTooBig) {
	int obj = 300;
	EXPECT_EQ(DICTIONARY_INVALID_ARG, Dictionary_Add(&dict, "pineapple", &obj, 4));
}

TEST_F(DictionaryTest, ObjectSizeTooBig) {
	std::string s = "ObjectSizeTooBig";
	EXPECT_EQ(DICTIONARY_INVALID_ARG, Dictionary_Add(&dict, "peach", &s, s.length()));
}

