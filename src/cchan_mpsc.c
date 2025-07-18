#include "cchan_mpsc.h"

#include <stdlib.h>

#include "utringbuffer.h"

void cchan_mpsc_new(CchanMpsc **self, uint32_t capacity, UT_icd *icd) {
  *self = (CchanMpsc *)malloc(sizeof(CchanMpsc));
  if (*self == NULL) {
    exit(-1);
  }
  utringbuffer_new((*self)->ringbuffer, capacity, icd);
  (*self)->read_index = 0;
  (*self)->lag_count = 0;
}

void cchan_mpsc_free(CchanMpsc *self) {
  utringbuffer_free(self->ringbuffer);
  self->ringbuffer = NULL;
  free(self);
}

void cchan_mpsc_clear(CchanMpsc *self) {
  utringbuffer_clear(self->ringbuffer);
  self->read_index = 0;
}

void cchan_mpsc_send(CchanMpsc *self, const void *message) {
  bool is_full = utringbuffer_full(self->ringbuffer);
  utringbuffer_push_back(self->ringbuffer, message);
  if (is_full) {
    if (self->read_index > 0) {
      self->read_index--;
    } else {
      self->lag_count++;
    }
  }
}

void *cchan_mpsc_receive(CchanMpsc *self, uint32_t *lag_count) {
  void *message = utringbuffer_eltptr(self->ringbuffer, self->read_index);
  if (message != NULL) {
    self->read_index++;
    *lag_count = self->lag_count;
    self->lag_count = 0;
  }
  return message;
}

uint32_t cchan_mpsc_length(const CchanMpsc *self) {
  return utringbuffer_len(self->ringbuffer) - self->read_index;
}

uint32_t cchan_mpsc_lag_count(const CchanMpsc *self) { return self->lag_count; }

bool cchan_mpsc_empty(const CchanMpsc *self) {
  return utringbuffer_len(self->ringbuffer) == self->read_index;
}

bool cchan_mpsc_full(const CchanMpsc *self) {
  return utringbuffer_full(self->ringbuffer) && self->read_index == 0;
}
