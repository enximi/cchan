#include "cchan_oneshot.h"

void cchan_oneshot_new(CchanOneshot **self, UT_icd *icd) {
  *self = malloc(sizeof(CchanOneshot));
  utringbuffer_new((*self)->ringbuffer, 1, icd);
  (*self)->received = false;
}

void cchan_oneshot_free(CchanOneshot *self) {
  utringbuffer_free(self->ringbuffer);
  self->ringbuffer = NULL;
  free(self);
}

bool cchan_oneshot_send(CchanOneshot *self, const void *message) {
  if (cchan_oneshot_sent(self)) {
    return false;
  }
  utringbuffer_push_back(self->ringbuffer, message);
  return true;
}

void *cchan_oneshot_receive(CchanOneshot *self) {
  void *message = utringbuffer_eltptr(self->ringbuffer, 0);
  if (message != NULL) {
    self->received = true;
  }
  return message;
}

bool cchan_oneshot_sent(const CchanOneshot *self) {
  return utringbuffer_len(self->ringbuffer) > 0;
}

bool cchan_oneshot_received(const CchanOneshot *self) { return self->received; }
