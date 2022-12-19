
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <assert.h>

long long get_timestamp_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long timestamp_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000.0;
    return timestamp_ms;
}

#define DEBOUNCE_MILLIS get_timestamp_ms
#define DEBOUNCE_IMPL
#include "debounce.h"

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void test_debounce_notimer()
{
    printf("test debounce no_timer\n");
    struct debounce_t signal = {};

    debounce_init(&signal, 1);
    assert(!debounce_notimer(&signal, 0) && "debounce should be false");
    // assert(!debounce_notimer(&signal, 1) && "debounce should be false");
    assert(debounce_notimer(&signal, 1) && "debounce should be true");

    printf("testing if if triggers after 5 inputs\n");
    debounce_init(&signal, 5);
    assert(!debounce_notimer(&signal, 0) && "debounce should be false"); // Add a message argument
    assert(!debounce_notimer(&signal, 1) && "debounce should be false");
    assert(!debounce_notimer(&signal, 1) && "debounce should be false");
    assert(!debounce_notimer(&signal, 1) && "debounce should be false");
    assert(!debounce_notimer(&signal, 1) && "debounce should be false");
    assert(debounce_notimer(&signal, 1) && "debounce should be true");
    printf("succesfully went from 0 to 1\n");
}

int main()
{

    struct debounce_t signal = {};
    debounce_init(&signal, 20);

    printf("%d\n", debounce(&signal, 0));
    printf("%d\n", debounce(&signal, 1));
    msleep(3);
    printf("%d\n", debounce(&signal, 1));
    msleep(20);
    printf("%d\n", debounce(&signal, 1));
    sleep(1);
    printf("%d\n\n", debounce(&signal, 1));

    int res = 1;
    while (res)
    {
        res = debounce(&signal, 0);
        printf("%d\n", res);
        msleep(1);
    }

    // res = debounce(&signal,1);
    printf("%d\n", debounce(&signal, 1));
    msleep(1);
    printf("%d\n", debounce(&signal, 1));
    msleep(6);
    printf("%d\n", debounce(&signal, 0));
    msleep(1);
    printf("%d\n", debounce(&signal, 1));
    msleep(1);
    printf("%d\n", debounce(&signal, 0));
    msleep(1);

    res = 0;
    while (!res)
    {
        res = debounce(&signal, 1);
        printf("%d\n", res);
        msleep(1);
    }

    test_debounce_notimer();

    return 0;
}