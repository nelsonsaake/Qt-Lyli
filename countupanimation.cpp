#include "countupanimation.h"



CountUpAnimation::CountUpAnimation(int duration, int count, int step)
    : count(count), step(step), ticker(duration, count/step)
{
    connect(&ticker,
            &Ticker::ticked,
            this,
            &CountUpAnimation::CountUp);
}

void CountUpAnimation::setCount(int count){
    this->count = count;
}

void CountUpAnimation::startAnimation(){
    ticker.forward();
    emit started();
}

void CountUpAnimation::CountUp(int tickCount){
    QString txt = QString::number(tickCount*step);
    if(tickCount<=count) emit update(txt);
    else emit finished();
}
