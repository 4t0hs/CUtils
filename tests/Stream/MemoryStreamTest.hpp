#pragma once

#include "gtest/gtest.h"
#include "Stream/MemoryStream.h"

class MemoryStreamTest : public ::testing::Test {
protected:
	MemoryStreamTest() {}
	virtual ~MemoryStreamTest() {}
	virtual void SetUp() {
		MemoryStream_Init(&memoryStream);
	}
	virtual void TearDown() {
		MemoryStream_Destroy(&memoryStream);
	}

	MemoryStream memoryStream;
};

TEST_F(MemoryStreamTest, Init) {
	EXPECT_NE(nullptr, memoryStream.stream);
	EXPECT_NE(nullptr, memoryStream.buffer);
	EXPECT_EQ(0, memoryStream.position);
	EXPECT_EQ(0, memoryStream.size);
}

TEST_F(MemoryStreamTest, Write) {
	const char* data = "Hello, World!";
	size_t dataSize = strlen(data);
	ssize_t written = MemoryStream_Write(&memoryStream, data, dataSize);
	EXPECT_EQ(dataSize, written);
	EXPECT_EQ(dataSize, memoryStream.size);
}

TEST_F(MemoryStreamTest, Read) {
	const char* data = "Hello, World!";
	size_t dataSize = strlen(data);
	MemoryStream_Write(&memoryStream, data, dataSize);
	char buffer[dataSize + 1] = {0};
	size_t bytesRead = MemoryStream_Read(&memoryStream, buffer, dataSize);
	EXPECT_EQ(dataSize, bytesRead);
	EXPECT_STREQ(data, buffer);
}

TEST_F(MemoryStreamTest, Seek) {
	const char* data = "Hello, World!";
	size_t dataSize = strlen(data);
	MemoryStream_Write(&memoryStream, data, dataSize);
	int ret = MemoryStream_Seek(&memoryStream, 7, SEEK_SET);
	EXPECT_EQ(0, ret);
	char buffer[6] = {0};
	size_t bytesRead = MemoryStream_Read(&memoryStream, buffer, 5);
	EXPECT_EQ(5, bytesRead);
	EXPECT_STREQ("World", buffer);
}

TEST_F(MemoryStreamTest, GetSize) {
	const char* data = "Hello, World!";
	size_t dataSize = strlen(data);
	MemoryStream_Write(&memoryStream, data, dataSize);
	size_t size = MemoryStream_GetSize(&memoryStream);
	EXPECT_EQ(dataSize, size);
}

TEST_F(MemoryStreamTest, SizeAfterSeek) {
	const char* data = "Hello, World!";
	size_t dataSize = strlen(data);

	MemoryStream_Write(&memoryStream, data, dataSize);
	MemoryStream_Seek(&memoryStream, -1, SEEK_CUR);
	MemoryStream_Write(&memoryStream, data, dataSize);

	size_t size = MemoryStream_GetSize(&memoryStream);
	size_t expected = dataSize * 2 - 1;

	EXPECT_EQ(expected, size);
}

TEST_F(MemoryStreamTest, RefBuffer) {
	const char* data = "Hello, World!";
	size_t dataSize = strlen(data);
	MemoryStream_Write(&memoryStream, data, dataSize);
	const void* buffer = MemoryStream_RefBuffer(&memoryStream);
	EXPECT_NE(nullptr, buffer);
}

