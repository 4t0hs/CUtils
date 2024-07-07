#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "PublishContent.h"
	typedef enum SubscriberReply {
		SUBSCRIBER_ACK = 0,
		SUBSCRIBER_NACK,
	} SubscriberReply;

	typedef SubscriberReply(*SubscriberUpdate)(const PublishContent_t *publish, void *userData);

	typedef struct Subscriber_t {
		SubscriberUpdate update;
		void *userData;
	} Subscriber_t;

	void Subscriber_Init(Subscriber_t *self, SubscriberUpdate update, void *userData);

	SubscriberReply Subscriber_Update(Subscriber_t *self, const PublishContent_t *publish);

	void Subscriber_Destroy(Subscriber_t *self);
#ifdef __cplusplus
}
#endif

