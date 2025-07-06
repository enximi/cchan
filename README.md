# cchan

[English](README.md) | [中文](README_zh.md)

## Overview

This project mimics tokio by implementing a set of channels for different communication modes in C language, including broadcast, multi-producer single-consumer, oneshot, and watch channels. Each channel type provides specific features to meet different use cases.

## File Structure

- `src/cchan_broadcast.c` and `src/cchan_broadcast.h`: Implements a broadcast channel, allowing multiple subscribers to receive the same message.
- `src/cchan_mpsc.c` and `src/cchan_mpsc.h`: Implements a multi-producer single-consumer channel, suitable for scenarios where multiple producers send messages to a single consumer.
- `src/cchan_oneshot.c` and `src/cchan_oneshot.h`: Implements a oneshot channel, suitable for one-time message passing.
- `src/cchan_watch.c` and `src/cchan_watch.h`: Implements a watch channel, allowing monitoring of message updates.

## Usage Instructions

### Broadcast Channel

```c
#include <stdint.h>
#include "cchan_broadcast.h"

int main() {
    CchanBroadcast *broadcast;
    uint32_t capacity = 10;
    UT_icd icd = {sizeof(int), NULL, NULL, NULL};

    // Create a broadcast channel
    cchan_broadcast_new(&broadcast, capacity, &icd);

    // Subscribe a subscriber
    uint32_t subscriber_id = cchan_broadcast_subscribe(broadcast);

    // Send a message
    int message = 42;
    cchan_broadcast_send(broadcast, &message);

    // Receive a message
    uint32_t lag_count;
    int *received_message = (int *)cchan_broadcast_receive(broadcast, subscriber_id, &lag_count);

    // Free the broadcast channel
    cchan_broadcast_free(broadcast);

    return 0;
}
```

### Multi-Producer Single-Consumer Channel

```c
#include <stdint.h>
#include "cchan_mpsc.h"

int main() {
    CchanMpsc *mpsc;
    uint32_t capacity = 10;
    UT_icd icd = {sizeof(int), NULL, NULL, NULL};

    // Create an MPSC channel
    cchan_mpsc_new(&mpsc, capacity, &icd);

    // Send a message
    int message = 42;
    cchan_mpsc_send(mpsc, &message);

    // Receive a message
    uint32_t lag_count;
    int *received_message = (int *)cchan_mpsc_receive(mpsc, &lag_count);

    // Free the MPSC channel
    cchan_mpsc_free(mpsc);

    return 0;
}
```

### Oneshot Channel

```c
#include "cchan_oneshot.h"

int main() {
    CchanOneshot *oneshot;
    UT_icd icd = {sizeof(int), NULL, NULL, NULL};

    // Create an Oneshot channel
    cchan_oneshot_new(&oneshot, &icd);

    // Send a message
    int message = 42;
    cchan_oneshot_send(oneshot, &message);

    // Receive a message
    int *received_message = (int *)cchan_oneshot_receive(oneshot);

    // Free the Oneshot channel
    cchan_oneshot_free(oneshot);

    return 0;
}
```

### Watch Channel

```c
#include "cchan_watch.h"

int main() {
    CchanWatch *watch;
    int initial_message = 42;
    UT_icd icd = {sizeof(int), NULL, NULL, NULL};

    // Create a Watch channel
    cchan_watch_new(&watch, &initial_message, &icd);

    // Send an update message
    int new_message = 43;
    cchan_watch_send(watch, &new_message);

    // Receive a message
    int *received_message = (int *)cchan_watch_receive(watch);

    // Free the Watch channel
    cchan_watch_free(watch);

    return 0;
}
```
