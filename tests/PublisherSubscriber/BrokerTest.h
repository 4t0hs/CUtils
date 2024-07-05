#pragma once

#include "gtest/gtest.h"
#include "PublisherSubscriber/Broker.h"

class BrokerTest : public::testing::Test {
protected:
	Broker_t broker;
	virtual void SetUp() {
		Broker_Init(&broker, 4);
	}
	virtual void TearDown() {
		Broker_Destroy(&broker);
	}
	void Publish(PublishContent_t *content) {
		Broker_Publish(&broker, content);
	}
	SubscriptionAccountId Subscribe(Subscriber_t *s, PublishMessageAttribute p) {
		return Broker_Subscribe(&broker, s, p);
	}
	void Unsubscribe(SubscriptionAccountId id) {
		Broker_Unsubscribe(&broker, id);
	}
};


