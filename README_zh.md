# cchan

[English](README.md) | [中文](README_zh.md)

## 概述

此项目模仿 tokio 用 c 语言简单实现了一组用于不同通信模式的通道，包括广播、多生产者单消费者、一次性和监视通道。每种通道类型都提供了特定的功能以满足不同的使用场景。

## 文件结构

- `src/cchan_broadcast.c` 和 `src/cchan_broadcast.h`: 实现了广播通道，允许多个订阅者接收相同的消息。
- `src/cchan_mpsc.c` 和 `src/cchan_mpsc.h`: 实现了多生产者单消费者通道，适用于多个生产者发送消息到单个消费者的场景。
- `src/cchan_oneshot.c` 和 `src/cchan_oneshot.h`: 实现了一次性通道，适用于一次性消息传递。
- `src/cchan_watch.c` 和 `src/cchan_watch.h`: 实现了监视通道，允许监视消息的更新。

## 使用说明

### 广播通道

```c
#include <stdint.h>
#include "cchan_broadcast.h"

int main() {
    CchanBroadcast *broadcast;
    uint32_t capacity = 10;
    UT_icd icd = {sizeof(int), NULL, NULL, NULL};

    // 创建广播通道
    cchan_broadcast_new(&broadcast, capacity, &icd);

    // 订阅者订阅
    uint32_t subscriber_id = cchan_broadcast_subscribe(broadcast);

    // 发送消息
    int message = 42;
    cchan_broadcast_send(broadcast, &message);

    // 接收消息
    uint32_t lag_count;
    int *received_message = (int *)cchan_broadcast_receive(broadcast, subscriber_id, &lag_count);

    // 释放广播通道
    cchan_broadcast_free(broadcast);

    return 0;
}
```

### 多生产者单消费者通道

```c
#include <stdint.h>
#include "cchan_mpsc.h"

int main() {
    CchanMpsc *mpsc;
    uint32_t capacity = 10;
    UT_icd icd = {sizeof(int), NULL, NULL, NULL};

    // 创建MPSC通道
    cchan_mpsc_new(&mpsc, capacity, &icd);

    // 发送消息
    int message = 42;
    cchan_mpsc_send(mpsc, &message);

    // 接收消息
    uint32_t lag_count;
    int *received_message = (int *)cchan_mpsc_receive(mpsc, &lag_count);

    // 释放MPSC通道
    cchan_mpsc_free(mpsc);

    return 0;
}
```

### 一次性通道

```c
#include "cchan_oneshot.h"

int main() {
    CchanOneshot *oneshot;
    UT_icd icd = {sizeof(int), NULL, NULL, NULL};

    // 创建Oneshot通道
    cchan_oneshot_new(&oneshot, &icd);

    // 发送消息
    int message = 42;
    cchan_oneshot_send(oneshot, &message);

    // 接收消息
    int *received_message = (int *)cchan_oneshot_receive(oneshot);

    // 释放Oneshot通道
    cchan_oneshot_free(oneshot);

    return 0;
}
```

### 监视通道

```c
#include "cchan_watch.h"

int main() {
    CchanWatch *watch;
    int initial_message = 42;
    UT_icd icd = {sizeof(int), NULL, NULL, NULL};

    // 创建Watch通道
    cchan_watch_new(&watch, &initial_message, &icd);

    // 发送更新消息
    int new_message = 43;
    cchan_watch_send(watch, &new_message);

    // 接收消息
    int *received_message = (int *)cchan_watch_receive(watch);

    // 释放Watch通道
    cchan_watch_free(watch);

    return 0;
}
```
