#ifndef SINGLETICKER_H
#define SINGLETICKER_H

#include "worker.h"

#include <QObject>
#include <QTimer>


/**
 * Ticker
 *
 * Ticker emits a ticked signal with a integer value
 * the integer value start at zero increases with every tick
 *
 * Ticker has two mode,
 * 1. ticker stops ticking once given value is reached
 * 2. ticker ticks until stop() is called
 *
 * It can be used to drive an animation
 * That is after every tick, the animation advance to
 * the next state
 *
 * It can be used to as a trigger to manually update ui.
 * Allowing the ui to be updated everytime an event occurs
 * can slow down the ui, so you can pile up the updates and
 * show them only when a ticked signal is received.
*/
class Ticker: public QObject
{

    Q_OBJECT

public:
    Ticker(int duration=100, int stopValue=1, int delay=0);

    // setter for duration
    void setDuration(int stopValue);

    // setter for value
    void setValue(int stopValue);

    // setter for tickInterval
    void setForeverTickModeTickInterval(int stopValue);

protected:
    // calculates or returns tickInterval depending on the mode
    int tickInterval();

    // reset tickCount(integer value emitted with every tick)
    // use delay if set, i.e. delay the first tick or
    // start tick timer
    void startTicking();

private slots:
    // increment the tickCount
    // send out ticked signal
    //
    // start timer, once timer hits zero tick would be called
    // it setups some kinda loop, once timer finishes it
    // is started again
    //
    // in some cases the loop stop once a certain value is
    // reached
    void tick();

public slots:
    // start ticking
    // this ticking would end after a certain value is reached
    void forward();

    // start ticking
    // this kinda ticking would never end unless
    // manually triggered
    void start();

    // stop ticking, this is how you stop the
    // ticker started start()
    void stop();

signals:
    // signals a tick
    // and carries the tickCount value along
    void ticked(int);

private:
    // in that instance where ticking stops by itself
    // duration is the entire period in which ticker
    // would be ticking
    //
    // duration itself is not used, it is used
    // to calculate the interval between ticks
    int duration;

    // in that instance where ticking stops by itself
    // when stopValue == tickCount, ticking stops
    int stopValue;

    // current tick state
    int tickCount;

    // how long to wait before the first tick signal after
    // ticker has been started
    int delay;

    // if is set to true, ticking would never stop
    bool isForeverTickMode;

    // bool == true, if ticking is to stop
    bool isStopTick;

    // in that instance where ticking never stops
    // this is the interval between ticks
    int foreverTickModeTickInterval;

    // whenever timer times out, we call that a tick
    QTimer timer;
};


#endif // SINGLETICKER_H
