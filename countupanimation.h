#ifndef COUNTUPANIMATION_H
#define COUNTUPANIMATION_H

#include "ticker.h"

#include <QLabel>
#include <QObject>



class CountUpAnimation: public QObject
{
    Q_OBJECT
public:
    CountUpAnimation(int duration = 1000, int count=100, int step=1);

    void setCount(int count);

public slots:
    void startAnimation();

private slots:
    void CountUp(int tickCount);

signals:
    void started();
    void update(QString nxt);
    void finished();

private:
    int count;
    int step;

    Ticker ticker;
};

#endif // COUNTUPANIMATION_H
