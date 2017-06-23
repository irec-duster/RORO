#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include "../../msgbus.h"
#include "types/example_type.h"

msgbus_t bus;

static void* producer(void *p)
{
    int max_count = *(int*)p;
    const char *name = "test";

    static msgbus_topic_t topic; // this must be static
    example_t topic_buffer;
    msgbus_topic_create(&topic, &bus, &example_type, &topic_buffer, name);

    example_t x = {.counter = 1};
    int i;
    for (i = 0; i < max_count; i++) {
        printf("[publisher] writing %d on topic %s\n", x.counter, name);
        msgbus_topic_publish(&topic, &x);
        x.counter += 1;
        sleep(1);
    }
    printf("[publisher] stopped\n");
    return NULL;
}

static void *consumer(void *p)
{
    int consumer_number = *(int*)p;

    msgbus_subscriber_t sub;

    printf("[consumer %d] waiting for topic\n", consumer_number);
    assert(msgbus_topic_subscribe(&sub, &bus, "test", MSGBUS_TIMEOUT_NEVER));

    example_t x;
    while (1) {
        int timeout_s = 5;
        if (msgbus_subscriber_wait_for_update(&sub, timeout_s*1000000)) {
            msgbus_subscriber_read(&sub, &x);
            printf("[consumer %d] read %d on topic\n", consumer_number, x.counter);
        } else {
            printf("[consumer %d] timed out after %d seconds\n", consumer_number, timeout_s);
            return NULL;
        }
    }

    return NULL;
}

int main(int argc, const char **argv)
{
    (void) argc;
    (void) argv;

    /* Create the message bus. */
    msgbus_init(&bus);


    /* Creates a few consumer threads. */
    pthread_t producer_thd, consumer_thd;
    int c1 = 1;
    pthread_create(&consumer_thd, NULL, consumer, &c1);
    int c2 = 2;
    pthread_create(&consumer_thd, NULL, consumer, &c2);
    int c3 = 3;
    pthread_create(&consumer_thd, NULL, consumer, &c3);

    sleep(1);

    /* Creates a producer thread */
    int p = 5;
    pthread_create(&producer_thd, NULL, producer, &p);

    while(1) {
        sleep(1);
    }
}
