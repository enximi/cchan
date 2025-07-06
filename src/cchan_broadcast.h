#ifndef CCHAN_BROADCAST_H
#define CCHAN_BROADCAST_H

#include <stdbool.h>
#include <stdint.h>

#include "uthash.h"
#include "utringbuffer.h"

typedef struct {
  uint32_t id;
  uint32_t read_index;
  uint32_t lag_count;
  UT_hash_handle hh;
} CchanBroadcastSubscriber;

typedef struct {
  UT_ringbuffer *ringbuffer;
  CchanBroadcastSubscriber *subscribers;
} CchanBroadcast;

void cchan_broadcast_new(CchanBroadcast **self, uint32_t capacity, UT_icd *icd);
void cchan_broadcast_free(CchanBroadcast *self);

void cchan_broadcast_clear(CchanBroadcast *self, uint32_t id);

uint32_t cchan_broadcast_subscribe(CchanBroadcast *self);
void cchan_broadcast_unsubscribe(CchanBroadcast *self, uint32_t id);

void cchan_broadcast_send(CchanBroadcast *self, const void *message);
void *cchan_broadcast_receive(CchanBroadcast *self, uint32_t id, uint32_t *lag_count);

uint32_t cchan_broadcast_length(const CchanBroadcast *self, uint32_t id);
uint32_t cchan_broadcast_lag_count(const CchanBroadcast *self, uint32_t id);
bool cchan_broadcast_empty(const CchanBroadcast *self, uint32_t id);
bool cchan_broadcast_full(const CchanBroadcast *self, uint32_t id);

#endif  // CCHAN_BROADCAST_H