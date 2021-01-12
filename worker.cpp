#include "worker.h"



Worker::Worker()
{
    // init mIsCancelled

    // means worker starts out not cancelled
    mIsCancelled = false;
}

Worker::~Worker(){}

bool Worker::isCancelled()
{
    // check is cancel state

    return mIsCancelled;
}

void Worker::clearIsCancel()
{
    // clear cancel
    // tell listeners cancel is cleared

    mIsCancelled = false;
    emit clearCancel();
}

void Worker::setIsCancelled()
{
    // set cancel
    // tell listeners cancel is set

    mIsCancelled = true;
    emit workCancelled();
}
