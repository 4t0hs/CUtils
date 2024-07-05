#pragma once

#include <vector>
#include "gtest/gtest.h"
#include "PublisherSubscriber/Subscriber.h"
#include "PublisherSubscriber/Publisher.h"

class Subject {
public:
	static constexpr PublishMessageAttribute ATTR1 = 1;
	static constexpr PublishMessageAttribute ATTR2 = 2;
	static constexpr PublishMessageAttribute ATTR3 = 3;
	static constexpr PublishMessageAttribute ATTR4 = 4;

	static constexpr PublishMessage MSG1 = 1;
	static constexpr PublishMessage MSG2 = 2;
	static constexpr PublishMessage MSG3 = 3;
	static constexpr PublishMessage MSG4 = 4;

	Publisher_t publisher;
	Subject() {
		Publisher_Init(&publisher, 4);
	}
	~Subject() {
		Publisher_Destroy(&publisher);
	}
	SubscriptionAccountId Subscribe(Subscriber_t *sub, PublishMessageAttribute attr) {
		return Publisher_Subscribe(&publisher, sub, attr);
	}
	void Publish(PublishMessage msg, PublishMessageAttribute attr) {
		PublishContent_t publish = { .message = msg, .attribute = attr };
		Publisher_Publish(&publisher, &publish);
	}
	void Unsubscribe(SubscriptionAccountId id) {
		Publisher_Unsubscribe(&publisher, id);
	}
};

class Observer {
public:
	Subscriber_t subscriber;
	SubscriberReply(*Update)(Observer *object, const PublishContent_t *publish) =
		[](Observer *object, const PublishContent_t *publish) {
		return SUBSCRIBER_ACK;
	};
	std::vector<PublishContent_t> publishes;
	int calledCount = 0;
	Observer() {
		Subscriber_Init(&subscriber, [](const PublishContent_t *publish, void *arg) {
			Observer *observer = (Observer *)arg;
			observer->calledCount++;
			observer->publishes.push_back(*publish);
			return observer->Update(observer, publish);
		}, this);
	}
	~Observer() {
		Subscriber_Destroy(&subscriber);
	}
};

class PubSubTest : public ::testing::Test {
protected:
	Subject subject;
	Observer observer1;
	Observer observer2;
	Observer observer3;
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(PubSubTest, Publish) {
	subject.Subscribe(&observer1.subscriber, subject.ATTR1);
	subject.Publish(subject.MSG1, subject.ATTR1);
	ASSERT_EQ(1, observer1.calledCount);
	ASSERT_EQ(1, observer1.publishes.size());
	EXPECT_EQ(subject.ATTR1, observer1.publishes[0].attribute);
	EXPECT_EQ(subject.MSG1, observer1.publishes[0].message);
}

TEST_F(PubSubTest, MultiplePublish) {
	subject.Subscribe(&observer1.subscriber, subject.ATTR1);
	subject.Publish(subject.MSG1, subject.ATTR1);
	subject.Publish(subject.MSG1, subject.ATTR1);
	ASSERT_EQ(2, observer1.calledCount);
	EXPECT_EQ(subject.ATTR1, observer1.publishes[0].attribute);
	EXPECT_EQ(subject.MSG1, observer1.publishes[0].message);
	EXPECT_EQ(subject.ATTR1, observer1.publishes[1].attribute);
	EXPECT_EQ(subject.MSG1, observer1.publishes[1].message);
}

TEST_F(PubSubTest, Subscribers) {
	subject.Subscribe(&observer1.subscriber, subject.ATTR1);
	subject.Subscribe(&observer2.subscriber, subject.ATTR1);
	subject.Publish(subject.MSG1, subject.ATTR1);
	ASSERT_EQ(1, observer1.calledCount);
	EXPECT_EQ(subject.ATTR1, observer1.publishes[0].attribute);
	EXPECT_EQ(subject.MSG1, observer1.publishes[0].message);
	ASSERT_EQ(1, observer2.calledCount);
	EXPECT_EQ(subject.ATTR1, observer2.publishes[0].attribute);
	EXPECT_EQ(subject.MSG1, observer2.publishes[0].message);
}

TEST_F(PubSubTest, DifferentAttribute) {
	subject.Subscribe(&observer1.subscriber, subject.ATTR1);
	subject.Publish(subject.MSG1, subject.ATTR2);
	EXPECT_EQ(0, observer1.calledCount);
}

TEST_F(PubSubTest, Republish) {
	observer1.Update =
		[](Observer *observer, const PublishContent_t *content) {
		if (observer->calledCount <= 1) {
			return SUBSCRIBER_NACK;
		}
		return SUBSCRIBER_ACK;
	};
	subject.Subscribe(&observer1.subscriber, subject.ATTR1);
	subject.Publish(subject.MSG1, subject.ATTR1);
	ASSERT_EQ(1, observer1.calledCount);
	subject.Publish(subject.MSG2, subject.ATTR2);
	EXPECT_EQ(2, observer1.calledCount);
}

TEST_F(PubSubTest, Unsubscribe) {
	SubscriptionAccountId id = subject.Subscribe(&observer1.subscriber, subject.ATTR1);
	subject.Unsubscribe(id);
	subject.Publish(subject.MSG1, subject.ATTR1);
	EXPECT_EQ(0, observer1.calledCount);
}

