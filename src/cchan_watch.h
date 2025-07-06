#ifndef CCHAN_WATCH_H
#define CCHAN_WATCH_H

#include <stdbool.h>
#include <stdint.h>

#include "utringbuffer.h"

typedef struct {
  UT_ringbuffer *ringbuffer;
  volatile uint32_t update_count;
} CchanWatch;

void cchan_watch_new(CchanWatch **self, void *initial_message, UT_icd *icd);
void cchan_watch_free(CchanWatch *self);

void cchan_watch_send(CchanWatch *self, const void *message);
void *cchan_watch_receive(CchanWatch *self);

bool cchan_watch_updated(const CchanWatch *self, uint32_t update_count);
uint32_t cchan_watch_update_count(const CchanWatch *self);

#endif  // CCHAN_WATCH_H