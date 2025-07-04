#ifndef CCHAN_BROADCAST_H
#define CCHAN_BROADCAST_H

#include <stdbool.h>

#include "cchan_def.h"
#include "uthash.h"
#include "utringbuffer.h"

// 订阅者
typedef struct {
  u32 id;
  u32 read_index;
  u32 lag_count;
  UT_hash_handle hh;
} CchanBroadcastSubscriber;

typedef struct {
  UT_ringbuffer *ringbuffer;
  CchanBroadcastSubscriber *subscribers;
} CchanBroadcast;

void cchan_broadcast_new(CchanBroadcast **self, u32 capacity, UT_icd *icd);
void cchan_broadcast_free(CchanBroadcast *self);

void cchan_broadcast_clear(CchanBroadcast *self, u32 id);

u32 cchan_broadcast_subscribe(CchanBroadcast *self);
void cchan_broadcast_unsubscribe(CchanBroadcast *self, u32 id);

void cchan_broadcast_send(CchanBroadcast *self, const void *message);
void *cchan_broadcast_receive(CchanBroadcast *self, u32 id, u32 *lag_count);

u32 cchan_broadcast_length(const CchanBroadcast *self, u32 id);
u32 cchan_broadcast_lag_count(const CchanBroadcast *self, u32 id);
bool cchan_broadcast_empty(const CchanBroadcast *self, u32 id);
bool cchan_broadcast_full(const CchanBroadcast *self, u32 id);

#endif  // CCHAN_BROADCAST_H