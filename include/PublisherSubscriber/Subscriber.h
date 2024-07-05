#pragma once

#include "PublishContent.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum SubscriberReply {
		SUBSCRIBER_ACK = 0,
		SUBSCRIBER_NACK,
	} SubscriberReply;

	typedef SubscriberReply(*SubscriberUpdate)(const PublishContent_t *publish, void *userData);

	typedef struct Subscriber_t {
		SubscriberUpdate update;
		void *userData;
	} Subscriber_t;

	void Subscriber_Init(Subscriber_t *object, SubscriberUpdate Update, void *userData);

	SubscriberReply Subscriber_Update(Subscriber_t *object, const PublishContent_t *publish);

	void Subscriber_Destroy(Subscriber_t *object);
#ifdef __cplusplus
}
#endif

