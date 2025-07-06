#ifndef CCHAN_MPSC_H
#define CCHAN_MPSC_H

#include <stdbool.h>
#include <stdint.h>

#include "utringbuffer.h"

typedef struct {
  UT_ringbuffer *ringbuffer;
  volatile uint32_t read_index;
  volatile uint32_t lag_count;
} CchanMpsc;

void cchan_mpsc_new(CchanMpsc **self, uint32_t capacity, UT_icd *icd);
void cchan_mpsc_free(CchanMpsc *self);

void cchan_mpsc_clear(CchanMpsc *self);

void cchan_mpsc_send(CchanMpsc *self, const void *message);
void *cchan_mpsc_receive(CchanMpsc *self, uint32_t *lag_count);

uint32_t cchan_mpsc_length(const CchanMpsc *self);
uint32_t cchan_mpsc_lag_count(const CchanMpsc *self);
bool cchan_mpsc_empty(const CchanMpsc *self);
bool cchan_mpsc_full(const CchanMpsc *self);

#endif  // CCHAN_MPSC_H