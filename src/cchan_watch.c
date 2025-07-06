#include "cchan_watch.h"

#include <stdlib.h>

void cchan_watch_new(CchanWatch **self, void *initial_message, UT_icd *icd) {
  *self = (CchanWatch *)malloc(sizeof(CchanWatch));
  if (*self == NULL) {
    exit(-1);
  }
  utringbuffer_new((*self)->ringbuffer, 1, icd);
  utringbuffer_push_back((*self)->ringbuffer, initial_message);
  (*self)->update_count = 0;
}

void cchan_watch_free(CchanWatch *self) {
  utringbuffer_free(self->ringbuffer);
  self->ringbuffer = NULL;
  free(self);
}

void cchan_watch_send(CchanWatch *self, const void *message) {
  utringbuffer_push_back(self->ringbuffer, message);
  self->update_count++;
}

void *cchan_watch_receive(CchanWatch *self) {
  return utringbuffer_eltptr(self->ringbuffer, 0);
}

bool cchan_watch_updated(const CchanWatch *self, uint32_t update_count) {
  return self->update_count != update_count;
}

uint32_t cchan_watch_update_count(const CchanWatch *self) {
  return self->update_count;
}
