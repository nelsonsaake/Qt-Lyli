#include "ticker.h"



Ticker::Ticker(int duration, int value, int delay)
    : duration(duration), stopValue(value), delay(delay)
{
    // init

    tickCount = 0;
    isForeverTickMode = false;
    isStopTick = false;
    foreverTickModeTickInterval = 100;

    // whenever timer times-out, we call tick
    //
    // ticker emits ticked signals at regular intervals
    // when timer times-out, the signal is emitted and
    // the timer is started again
    // this would keep the signal going out continuously
    connect(&timer,
            &QTimer::timeout,
            this,
            &Ticker::tick);
}

void Ticker::setDuration(int value)
{
    // duration setter

    duration = value;
}

void Ticker::setValue(int value)
{
    // stopValue setter

    this->stopValue = value;
}

void Ticker::setForeverTickModeTickInterval(int value)
{
    // foreverTickModeTickInterval setter

    foreverTickModeTickInterval = value;
}

int Ticker::tickInterval()
{
    // calculates or returns tickInterval depending on the mode
    // when ticking stop at some point, it calculates

    // when ticking forever
    if(isForeverTickMode) return foreverTickModeTickInterval;

    // otherwise
    else if(duration==0 || stopValue==0) return 0;
    return duration/stopValue;
}

void Ticker::startTicking()
{
    // kick start the ticking
    // ticking = signal emitted at regular interval

    // once tick() is called, it will handle the rest

    // the first tick will increment the value
    // and emit it
    // => tick count will start at '0'
    tickCount = -1;
    isStopTick = false;

    if(delay>0) timer.start(delay);
    else tick();
}

void Ticker::tick()
{
    // tick emit ticked signal with an int value
    // setup the next tick if neccessary

    // tick sets up the next tick by starting timer
    // once the timer times out, it would call tick

    // consequently it creates a loop that emits a
    // signal at regular interval

    //
    if(isStopTick) return;

    // tickCount = integer vale emitted
    tickCount++;
    emit ticked(tickCount);

    // setup tick depenging the current mode of
    // ticker
    // foreverTickMode = tick stops only when
    // stop() is called
    if(isForeverTickMode){
        timer.start(tickInterval());
    } else{
        // there are two criteria used to determine
        // when to stop ticking once one is reached,
        // the other is reached
        if(tickCount*tickInterval()<duration){
            timer.start(tickInterval());
        }
    }
}

void Ticker::forward()
{
    // starts ticking that would end after some time

    if(duration==0||stopValue==0) return;
    isForeverTickMode = false;
    startTicking();
}

void Ticker::start()
{
    // starts ticking that will only end with stop()

    isForeverTickMode = true;
    isStopTick = false;
    startTicking();
}

void Ticker::stop()
{
    // tick() is triggered
    // to stop it a check value used

    isStopTick = true;
}
