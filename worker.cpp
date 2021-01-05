#include "worker.h"



Worker::Worker()
{
    mIsCancelled = false;
}

Worker::~Worker(){}

bool Worker::isCancelled(){
    return mIsCancelled;
}

void Worker::clearIsCancel(){
    mIsCancelled = false;
    emit clearCancel();
}

void Worker::setIsCancelled(){
    mIsCancelled = true;
    emit workCancelled();
}
