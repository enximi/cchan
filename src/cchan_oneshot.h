#ifndef CCHAN_ONESHOT_H
#define CCHAN_ONESHOT_H

#include <stdbool.h>

#include "utringbuffer.h"

typedef struct {
  UT_ringbuffer *ringbuffer;
  volatile bool received;
} CchanOneshot;

void cchan_oneshot_new(CchanOneshot **self, UT_icd *icd);
void cchan_oneshot_free(CchanOneshot *self);

bool cchan_oneshot_send(CchanOneshot *self, const void *message);
void *cchan_oneshot_receive(CchanOneshot *self);

bool cchan_oneshot_sent(const CchanOneshot *self);
bool cchan_oneshot_received(const CchanOneshot *self);

#endif  // CCHAN_ONESHOT_H