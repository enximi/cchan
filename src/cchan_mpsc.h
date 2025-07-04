#ifndef CCHAN_MPSC_H
#define CCHAN_MPSC_H

#include <stdbool.h>

#include "cchan_def.h"
#include "utringbuffer.h"

typedef struct {
  UT_ringbuffer *ringbuffer;
  volatile u32 read_index;
  volatile u32 lag_count;
} CchanMpsc;

void cchan_mpsc_new(CchanMpsc **self, u32 capacity, UT_icd *icd);
void cchan_mpsc_free(CchanMpsc *self);

void cchan_mpsc_clear(CchanMpsc *self);

void cchan_mpsc_send(CchanMpsc *self, const void *message);
void *cchan_mpsc_receive(CchanMpsc *self, u32 *lag_count);

u32 cchan_mpsc_length(const CchanMpsc *self);
u32 cchan_mpsc_lag_count(const CchanMpsc *self);
bool cchan_mpsc_empty(const CchanMpsc *self);
bool cchan_mpsc_full(const CchanMpsc *self);

#endif  // CCHAN_MPSC_H