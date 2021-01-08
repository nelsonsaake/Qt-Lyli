#include "countupanimation.h"



CountUpAnimation::CountUpAnimation(int duration, int count, int step)
    : maxCount(count), step(step), ticker(duration, count/step)
{
    // listen to ticker to count up
    connect(&ticker,
            &Ticker::ticked,
            this,
            &CountUpAnimation::CountUp);
}

void CountUpAnimation::startAnimation()
{
    // start the ticker
    // once the ticker is started it would emit signals that would drive the animation

    // ticker.start() would also start the ticker
    // but ticker.start() would never stop ticking
    // ticker.forward() stops after the specified duration

    ticker.forward();
    emit started();
}

void CountUpAnimation::CountUp(int tickCount)
{
    // convert the current count to string and emit the value
    //
    // CountUp doesn't really increment anything,
    // it takes current count from the ticker via
    // signals and slots connected in the constructor
    //
    // tickCount*step is only neccessary because of
    // count/step in the constructor initialization

    QString txt = QString::number(tickCount*step);
    if(tickCount<=maxCount) emit update(txt);
    else emit finished();
}
