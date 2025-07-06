#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uticds.h"


// Include all channel types
#include "../src/cchan_broadcast.h"
#include "../src/cchan_mpsc.h"
#include "../src/cchan_oneshot.h"
#include "../src/cchan_watch.h"

// Demonstrate usage of broadcast channel
void demo_broadcast() {
  printf("=== Broadcast Channel Demo ===\n");

  CchanBroadcast *broadcast = NULL;
  cchan_broadcast_new(&broadcast, 10, &ut_int_icd);

  // Subscribe two receivers
  uint32_t subscriber1 = cchan_broadcast_subscribe(broadcast);
  uint32_t subscriber2 = cchan_broadcast_subscribe(broadcast);

  printf("Subscriber1 ID: %u\n", subscriber1);
  printf("Subscriber2 ID: %u\n", subscriber2);

  // Send messages
  for (int i = 1; i <= 11; i++) {
    cchan_broadcast_send(broadcast, &i);
    printf("Broadcast message: %d\n", i);
  }

  // Receive messages
  printf("\nMessages received by subscriber1:\n");
  uint32_t lag_count;
  for (int i = 0; i < 5; i++) {
    int *msg = (int *)cchan_broadcast_receive(broadcast, subscriber1, &lag_count);
    if (msg) {
      printf("Received: %d (lag count: %u)\n", *msg, lag_count);
    }
  }

  printf("\nMessages received by subscriber2:\n");
  for (int i = 0; i < 5; i++) {
    int *msg = (int *)cchan_broadcast_receive(broadcast, subscriber2, &lag_count);
    if (msg) {
      printf("Received: %d (lag count: %u)\n", *msg, lag_count);
    }
  }

  // Unsubscribe
  cchan_broadcast_unsubscribe(broadcast, subscriber1);
  cchan_broadcast_unsubscribe(broadcast, subscriber2);

  cchan_broadcast_free(broadcast);
  printf("\n");
}

// Demonstrate usage of MPSC channel
void demo_mpsc() {
  printf("=== Multi-Producer Single-Consumer Channel (MPSC) Demo ===\n");

  CchanMpsc *mpsc = NULL;
  cchan_mpsc_new(&mpsc, 10, &ut_int_icd);

  // Send messages (simulating multiple producers)
  for (int i = 1; i <= 5; i++) {
    cchan_mpsc_send(mpsc, &i);
    printf("Send message: %d\n", i);
  }

  printf("Channel length: %u\n", cchan_mpsc_length(mpsc));

  // Receive messages
  printf("\nReceived messages:\n");
  uint32_t lag_count;
  while (!cchan_mpsc_empty(mpsc)) {
    int *msg = (int *)cchan_mpsc_receive(mpsc, &lag_count);
    if (msg) {
      printf("Received: %d (lag count: %u)\n", *msg, lag_count);
    }
  }

  cchan_mpsc_free(mpsc);
  printf("\n");
}

// Demonstrate usage of oneshot channel
void demo_oneshot() {
  printf("=== Oneshot Channel Demo ===\n");

  CchanOneshot *oneshot = NULL;
  cchan_oneshot_new(&oneshot, &ut_int_icd);

  int value = 42;
  printf("Send status: %s\n", cchan_oneshot_sent(oneshot) ? "Sent" : "Not sent");
  printf("Receive status: %s\n", cchan_oneshot_received(oneshot) ? "Received" : "Not received");

  // Send message
  bool sent = cchan_oneshot_send(oneshot, &value);
  printf("Send message %d: %s\n", value, sent ? "Success" : "Failed");

  printf("Send status: %s\n", cchan_oneshot_sent(oneshot) ? "Sent" : "Not sent");

  // Receive message
  int *received = (int *)cchan_oneshot_receive(oneshot);
  if (received) {
    printf("Received message: %d\n", *received);
  }

  printf("Receive status: %s\n", cchan_oneshot_received(oneshot) ? "Received" : "Not received");

  // Try to send again (should fail)
  int value2 = 100;
  bool sent2 = cchan_oneshot_send(oneshot, &value2);
  printf("Try to send again %d: %s\n", value2, sent2 ? "Success" : "Failed");

  cchan_oneshot_free(oneshot);
  printf("\n");
}

// Demonstrate usage of watch channel
void demo_watch() {
  printf("=== Watch Channel Demo ===\n");

  CchanWatch *watch = NULL;
  int initial_value = 0;
  cchan_watch_new(&watch, &initial_value, &ut_int_icd);

  // Get initial value
  int *current = (int *)cchan_watch_receive(watch);
  printf("Initial value: %d\n", current ? *current : -1);

  uint32_t update_count = cchan_watch_update_count(watch);
  printf("Update count: %u\n", update_count);

  // Send new values
  for (int i = 10; i <= 13; i++) {
    cchan_watch_send(watch, &i);
    printf("Send new value: %d\n", i);

    // Check if updated
    if (cchan_watch_updated(watch, update_count)) {
      printf("Update detected!\n");
      update_count = cchan_watch_update_count(watch);
    }

    // Get current value
    current = (int *)cchan_watch_receive(watch);
    printf("Current value: %d\n", current ? *current : -1);
  }

  printf("Final update count: %u\n", cchan_watch_update_count(watch));

  cchan_watch_free(watch);
  printf("\n");
}

int main() {
  printf("CChan Channel Library Usage Examples\n");
  printf("====================================\n\n");

  // Demonstrate all channel types
  demo_broadcast();
  demo_mpsc();
  demo_oneshot();
  demo_watch();

  printf("All demonstrations completed!\n");
  return 0;
}