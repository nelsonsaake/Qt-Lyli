#ifndef COUNTUPANIMATION_H
#define COUNTUPANIMATION_H

#include "ticker.h"

#include <QLabel>
#include <QObject>

/**
 * CountUpAnimation
 *
 * CountUpAnimation emits integer values incremental over a given duration,
 * The value starts from zero, the end value is given
 * The step is also given, that by how much the value is increase at a single instance
*/
class CountUpAnimation: public QObject
{

    Q_OBJECT

public:
    // constructor
    CountUpAnimation(int duration = 1000, int maxCount=100, int step=1);

public slots:
    void startAnimation();

private slots:
    // this increments the tickCount and emits the value
    void CountUp(int tickCount);

signals:
    // signal the start of animation
    void started();

    // emits integer value as text
    void update(QString nxt);

    // signal finish of animation
    void finished();

private:
    // the animation start incrementing from zero this maxCount
    int maxCount;

    // for this is how much the count is incremented
    int step;

    // ticker is used to drive the animation
    // is emits signals that signifies moving from one state to another
    Ticker ticker;
};

#endif // COUNTUPANIMATION_H
