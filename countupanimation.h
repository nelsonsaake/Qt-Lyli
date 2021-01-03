#ifndef COUNTUPANIMATION_H
#define COUNTUPANIMATION_H

#include "ticker.h"

#include <QLabel>
#include <QObject>



class CountUpAnimation: public QObject
{
    Q_OBJECT
public:
    CountUpAnimation(int duration = 1000, int count=100, int step=1)
        : count(count), step(step), ticker(duration, count/step)
    {
        connect(&ticker,
                &Ticker::ticked,
                this,
                &CountUpAnimation::CountUp);
    }

    void setCount(int count){
        this->count = count;
    }

public slots:
    void startAnimation(){
        ticker.forward();
        emit started();
    }

private slots:
    void CountUp(int tickCount){
        QString txt = QString::number(tickCount*step);
        if(tickCount<=count) emit nextState(txt);
        else emit finished();
    }

signals:
    void started();
    void nextState(QString nxt);
    void finished();

private:
    int count;
    int step;

    Ticker ticker;
};

#endif // COUNTUPANIMATION_H
