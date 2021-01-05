#include "ticker.h"







Ticker::Ticker(int duration, int value, int delay)
    : duration(duration), value(value), delay(delay){

    tickCount = 0;
    foreverTickMode = false;
    foreverTickStopped = false;
    foreverModeTickInterval = 100;

    connect(&timer,
            &QTimer::timeout,
            this,
            &Ticker::tick);
}

void Ticker::setDuration(int value)
{
    duration = value;
}

void Ticker::setValue(int value)
{
    this->value = value;
}

void Ticker::setTickInterval(int value){
    if(foreverModeTickInterval<100) return;
    foreverModeTickInterval = value;
}

int Ticker::tickInterval(){
    if(duration==0 || value==0) return 0;
    return duration/value;
}

void Ticker::startTicking(){
    // the first tick will increment the value
    // and emit it
    // => tick count will start at '0'
    tickCount = -1;

    if(delay>0) timer.start(delay);
    else tick();
}

void Ticker::tick(){
    tickCount++;
    emit ticked(tickCount);

    if(foreverTickMode){
        if(!foreverTickStopped){
            timer.start(foreverModeTickInterval);
        }
    } else{
        if(tickCount*tickInterval()<duration){
            int msec = tickInterval();
            timer.start(msec);
        }
    }
}

void Ticker::forward(){
    if(duration==0||value==0) return;
    foreverTickMode = false;
    startTicking();
}

void Ticker::start(){
    foreverTickMode = true;
    foreverTickStopped = false;
    startTicking();
}

void Ticker::stop(){
    foreverTickStopped = true;
}
