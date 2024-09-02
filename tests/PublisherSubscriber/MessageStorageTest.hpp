#pragma once

#include <string>
#include "gtest/gtest.h"
#include "PublisherSubscriber/MessageStorage.h"

class MessageStorageTest : public::testing::Test {
protected:
	static constexpr std::string_view msg1 = "MESSAGE1";
	static constexpr std::string_view msg2 = "MESSAGE2";
	static constexpr std::string_view msg3 = "MESSAGE3";
	static constexpr std::string_view msg4 = "MESSAGE4";
	static constexpr std::string_view msg5 = "MESSAGE5";

	MessageStorage_t messages;
	static constexpr int maxMessage = 4;
	virtual void SetUp() {
		MessageStorage_Init(&messages, sizeof(std::string_view), maxMessage);
	}
	virtual void TearDown() {
		MessageStorage_Destroy(&messages);
	}

	int Push(const std::string_view *adding) {
		return MessageStorage_Push(&messages, adding);
	}
	int Pop(std::string_view *buffer) {
		return MessageStorage_Pop(&messages, buffer);
	}
	const std::string_view *Peek() {
		return (const std::string_view *)MessageStorage_Peek(&messages);
	}
	void MoveLast() {
		MessageStorage_MoveLast(&messages);
	}
	void RemoveTop() {
		MessageStorage_RemoveTop(&messages);
	}
};

TEST_F(MessageStorageTest, PushPoP) {
	ASSERT_EQ(0, Push(&msg1));
	std::string_view str;
	ASSERT_EQ(0, Pop(&str));
	EXPECT_STREQ(msg1.data(), str.data());
}

TEST_F(MessageStorageTest, MultiplePushPop) {
	ASSERT_EQ(0, Push(&msg1));
	ASSERT_EQ(0, Push(&msg2));
	EXPECT_EQ(2, messages.count);
	std::string_view sv;
	ASSERT_EQ(0, Pop(&sv));
	EXPECT_STREQ(msg1.data(), sv.data());
	ASSERT_EQ(0, Pop(&sv));
	EXPECT_STREQ(msg2.data(), sv.data());
	EXPECT_EQ(0, messages.count);
}

TEST_F(MessageStorageTest, PeekAndMoveLast) {
	Push(&msg1);
	Push(&msg2);
	const std::string_view *sv = Peek();
	ASSERT_NE(nullptr, sv);
	EXPECT_STREQ(msg1.data(), sv->data());
	MoveLast();
	void *p = ((uint8_t *)messages.messages + (messages.bottom * messages.messageSize));
	sv = static_cast<std::string_view *>(p);
	EXPECT_STREQ(msg1.data(), sv->data());
}

TEST_F(MessageStorageTest, RemoveTop) {
	Push(&msg1);
	Push(&msg2);
	RemoveTop();
	EXPECT_EQ(1, messages.count);
	std::string_view sv;
	Pop(&sv);
	EXPECT_STREQ(msg2.data(), sv.data());
}

TEST_F(MessageStorageTest, BufferEmpty) {
	std::string_view sv;
	EXPECT_EQ(-1, Pop(nullptr));
	Push(&msg1);
	MoveLast();
	ASSERT_EQ(0, Pop(&sv));
	EXPECT_EQ(-1, Pop(&sv));
	EXPECT_EQ(nullptr, Peek());
	RemoveTop();
	EXPECT_EQ(0, messages.count);
}

TEST_F(MessageStorageTest, OverCapacity) {
	ASSERT_EQ(0, Push(&msg1));
	ASSERT_EQ(0, Push(&msg2));
	ASSERT_EQ(0, Push(&msg3));
	ASSERT_EQ(0, Push(&msg4));
	ASSERT_EQ(-1, Push(&msg1));
}

TEST_F(MessageStorageTest, RoundTheArray) {
	Push(&msg1);
	Push(&msg2);
	Push(&msg3);
	Push(&msg4);
	std::string_view sv;
	Pop(&sv);
	ASSERT_EQ(msg1.data(), sv.data());
	Push(&msg5);
	Pop(&sv);
	ASSERT_STREQ(msg2.data(), sv.data());
	Pop(&sv);
	ASSERT_STREQ(msg3.data(), sv.data());
	Pop(&sv);
	ASSERT_STREQ(msg4.data(), sv.data());
	Pop(&sv);
	ASSERT_STREQ(msg5.data(), sv.data());
}







