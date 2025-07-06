#include "cchan_broadcast.h"

#include "utringbuffer.h"

void cchan_broadcast_new(CchanBroadcast **self, uint32_t capacity, UT_icd *icd) {
  *self = malloc(sizeof(CchanBroadcast));
  utringbuffer_new((*self)->ringbuffer, capacity, icd);
  (*self)->subscribers = NULL;
}

void cchan_broadcast_free(CchanBroadcast *self) {
  utringbuffer_free(self->ringbuffer);
  self->ringbuffer = NULL;
  CchanBroadcastSubscriber *subscriber, *tmp;
  HASH_ITER(hh, self->subscribers, subscriber, tmp) {
    HASH_DEL(self->subscribers, subscriber);
    free(subscriber);
  }
  self->subscribers = NULL;
  free(self);
}

void cchan_broadcast_clear(CchanBroadcast *self, uint32_t id) {
  CchanBroadcastSubscriber *subscriber;
  HASH_FIND(hh, self->subscribers, &id, sizeof(uint32_t), subscriber);
  if (subscriber != NULL) {
    subscriber->read_index = utringbuffer_len(self->ringbuffer);
    subscriber->lag_count = 0;
  }
}

uint32_t cchan_broadcast_subscribe(CchanBroadcast *self) {
  uint32_t id = 0;
  CchanBroadcastSubscriber *subscriber = NULL;
  do {
    subscriber = NULL;
    HASH_FIND(hh, self->subscribers, &id, sizeof(uint32_t), subscriber);
    if (subscriber != NULL) {
      id++;
    }
  } while (subscriber != NULL);
  subscriber = malloc(sizeof(CchanBroadcastSubscriber));
  subscriber->id = id;
  subscriber->read_index = utringbuffer_len(self->ringbuffer);
  subscriber->lag_count = 0;
  HASH_ADD(hh, self->subscribers, id, sizeof(uint32_t), subscriber);
  return id;
}

void cchan_broadcast_unsubscribe(CchanBroadcast *self, uint32_t id) {
  CchanBroadcastSubscriber *subscriber;
  HASH_FIND(hh, self->subscribers, &id, sizeof(uint32_t), subscriber);
  if (subscriber != NULL) {
    HASH_DEL(self->subscribers, subscriber);
    free(subscriber);
  }
}

void cchan_broadcast_send(CchanBroadcast *self, const void *message) {
  bool is_full = utringbuffer_full(self->ringbuffer);
  utringbuffer_push_back(self->ringbuffer, message);
  if (!is_full) {
    return;
  }
  CchanBroadcastSubscriber *subscriber, *tmp;
  HASH_ITER(hh, self->subscribers, subscriber, tmp) {
    if (subscriber->read_index > 0) {
      subscriber->read_index--;
    } else {
      subscriber->lag_count++;
    }
  }
}

void *cchan_broadcast_receive(CchanBroadcast *self, uint32_t id, uint32_t *lag_count) {
  CchanBroadcastSubscriber *subscriber;
  HASH_FIND(hh, self->subscribers, &id, sizeof(uint32_t), subscriber);
  if (subscriber == NULL) {
    return NULL;
  }
  void *message = utringbuffer_eltptr(self->ringbuffer, subscriber->read_index);
  if (message == NULL) {
    return NULL;
  }
  subscriber->read_index++;
  *lag_count = subscriber->lag_count;
  subscriber->lag_count = 0;
  return message;
}

uint32_t cchan_broadcast_length(const CchanBroadcast *self, uint32_t id) {
  CchanBroadcastSubscriber *subscriber;
  HASH_FIND(hh, self->subscribers, &id, sizeof(uint32_t), subscriber);
  if (subscriber == NULL) {
    return 0;
  }
  return utringbuffer_len(self->ringbuffer) - subscriber->read_index;
}

uint32_t cchan_broadcast_lag_count(const CchanBroadcast *self, uint32_t id) {
  CchanBroadcastSubscriber *subscriber;
  HASH_FIND(hh, self->subscribers, &id, sizeof(uint32_t), subscriber);
  if (subscriber == NULL) {
    return 0;
  }
  return subscriber->lag_count;
}

bool cchan_broadcast_empty(const CchanBroadcast *self, uint32_t id) {
  CchanBroadcastSubscriber *subscriber;
  HASH_FIND(hh, self->subscribers, &id, sizeof(uint32_t), subscriber);
  if (subscriber == NULL) {
    return true;
  }
  return subscriber->read_index >= utringbuffer_len(self->ringbuffer);
}

bool cchan_broadcast_full(const CchanBroadcast *self, uint32_t id) {
  CchanBroadcastSubscriber *subscriber;
  HASH_FIND(hh, self->subscribers, &id, sizeof(uint32_t), subscriber);
  if (subscriber == NULL) {
    return false;
  }
  return utringbuffer_full(self->ringbuffer) && subscriber->read_index == 0;
}
