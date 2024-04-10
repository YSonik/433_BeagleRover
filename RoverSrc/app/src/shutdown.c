#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "shutdown.h"
#include "utils/file_utils.h"

static bool isInitialized = false;
static bool shutdownRequested = false;

pthread_mutex_t shutdown_lock;

void Shutdown_init()
{
    assert(!isInitialized);

    pthread_mutex_init(&shutdown_lock, NULL);

    isInitialized = true;
}

void Shutdown_request()
{
    pthread_mutex_lock(&shutdown_lock);
    shutdownRequested = true;
    pthread_mutex_unlock(&shutdown_lock);
}

bool Shutdown_isRequested()
{
    pthread_mutex_lock(&shutdown_lock);
    bool result = shutdownRequested;
    pthread_mutex_unlock(&shutdown_lock);

    return result;
}

void Shutdown_cleanup()
{
    assert(isInitialized);

    pthread_mutex_destroy(&shutdown_lock);

    isInitialized = false;
}