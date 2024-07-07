#pragma once
#include <errno.h>
#include "gtest/gtest.h"
#include "PublisherSubscriber/Subscription.h"

class SubscriptionTest : public::testing::Test {
protected:
	static constexpr int attr1 = 1;
	static constexpr int attr2 = 2;
	static constexpr int attr3 = 3;
	static constexpr int attr4 = 4;

	Subscription_t subscription;
	static constexpr int numAccounts = 4;
	virtual void SetUp() {
		Subscription_Init(&subscription, numAccounts);
	}
	virtual void TearDown() {
		Subscription_Destroy(&subscription);
	}

	SubscriptionAccountId Contract(Subscriber_t *subscriber, PublishMessageAttribute attribute) {
		return Subscription_Contract(&subscription, subscriber, attribute);
	}
	void Cancellation(SubscriptionAccountId id) {
		Subscription_Cancellation(&subscription, id);
	}
	ssize_t Match(PublishMessageAttribute attribute, SubscriptionAccountId matchedIds[], size_t bufferSize) {
		return Subscription_Match(&subscription, attribute, matchedIds, bufferSize);
	}
	size_t Count() {
		return Subscription_Count(&subscription);
	}
	SubscriptionAccount_t *GetAccount(SubscriptionAccountId id) {
		return Subscription_GetAccount(&subscription, id);
	}
	static SubscriberReply DummyUpdate(const PublishContent_t *publish, void *arg) {
		return SUBSCRIBER_ACK;
	}
	static int dummyUserData;
};

TEST_F(SubscriptionTest, Contract) {
	Subscriber_t user = { .update = (SubscriberUpdate)1, .userData = (void *)2 };
	SubscriptionAccountId userId = Contract(&user, attr1);
	ASSERT_NE(-1, userId);
	SubscriptionAccount_t *account = GetAccount(userId);
	ASSERT_NE(nullptr, account);
	EXPECT_EQ(userId, account->id);
	EXPECT_EQ(true, account->contracted);
	EXPECT_EQ(attr1, account->interestedPublish);
	EXPECT_EQ((SubscriberUpdate)1, account->subscriber.update);
	EXPECT_EQ((void *)2, account->subscriber.userData);
}

TEST_F(SubscriptionTest, Cancel) {
	Subscriber_t user;
	SubscriptionAccountId id = Contract(&user, attr1);
	ASSERT_NE(nullptr, GetAccount(id));
	Cancellation(id);
	EXPECT_EQ(nullptr, GetAccount(id));
}

TEST_F(SubscriptionTest, MultipleContracts) {
	Subscriber_t user1, user2, user3;
	SubscriptionAccountId user1Id = Contract(&user1, attr1);
	ASSERT_NE(-1, user1Id);
	SubscriptionAccountId user2Id = Contract(&user2, attr2);
	ASSERT_NE(-1, user2Id);
	SubscriptionAccountId user3Id = Contract(&user3, attr3);
	ASSERT_NE(-1, user3Id);

	EXPECT_NE(nullptr, GetAccount(user1Id));
	EXPECT_NE(nullptr, GetAccount(user3Id));
	EXPECT_NE(nullptr, GetAccount(user3Id));
}

TEST_F(SubscriptionTest, ExceedsNumAccounts) {
	Subscriber_t user1, user2, user3, user4, user5;
	ASSERT_NE(-1, Contract(&user1, attr1));
	ASSERT_NE(-1, Contract(&user2, attr2));
	ASSERT_NE(-1, Contract(&user3, attr3));
	ASSERT_NE(-1, Contract(&user4, attr4));
	ASSERT_EQ(-1, Contract(&user5, attr1));
}

TEST_F(SubscriptionTest, IdNotExists) {
	Subscriber_t user;
	SubscriptionAccountId id = Contract(&user, attr1);
	ASSERT_NE(-1, id);
	EXPECT_EQ(nullptr, GetAccount(id + 1));
}

TEST_F(SubscriptionTest, InvalidAPIsArgument) {
	Cancellation(numAccounts + 1);
	EXPECT_EQ(nullptr, GetAccount(numAccounts + 1));
}

TEST_F(SubscriptionTest, Matches) {
	Subscriber_t user1, user2, user3, user4;
	SubscriptionAccountId user1Id = Contract(&user1, attr1);
	SubscriptionAccountId user2Id = Contract(&user2, attr1);
	SubscriptionAccountId user3Id = Contract(&user3, attr2);
	SubscriptionAccountId user4Id = Contract(&user4, attr3);
	SubscriptionAccountId matchedIds[numAccounts];
	ASSERT_EQ(2, Match(attr1, matchedIds, numAccounts));
	EXPECT_EQ(user1Id, matchedIds[0]);
	EXPECT_EQ(user2Id, matchedIds[1]);
	ASSERT_EQ(1, Match(attr2, matchedIds, numAccounts));
	ASSERT_EQ(1, Match(attr3, matchedIds, numAccounts));
	ASSERT_EQ(0, Match(attr4, matchedIds, numAccounts));
}

TEST_F(SubscriptionTest, MatchedBufferOver) {
	Subscriber_t user1, user2;
	Contract(&user1, attr1);
	Contract(&user2, attr1);
	SubscriptionAccountId ids[1];
	EXPECT_EQ(-1, Match(attr1, ids, 1));
}

TEST_F(SubscriptionTest, count) {
	Subscriber_t user1, user2;
	Contract(&user1, attr1);
	Contract(&user2, attr1);
	EXPECT_EQ(2, Count());
}





