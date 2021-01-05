#ifndef SINGLETICKER_H
#define SINGLETICKER_H

#include "worker.h"

#include <QObject>
#include <QTimer>



class Ticker: public QObject
{
    Q_OBJECT
public:
    Ticker(int duration=100, int value=1, int delay=0);

    void setDuration(int value);

    void setValue(int value);

    void setTickInterval(int value);

protected:
    int tickInterval();

    void startTicking();

private slots:
    void tick();

public slots:
    void forward();

    void start();

    void stop();

signals:
    void ticked(int);

private:
    int duration;
    int value;
    int tickCount;
    int delay;

    bool foreverTickMode;
    bool foreverTickStopped;
    int foreverModeTickInterval;

    QTimer timer;
};


#endif // SINGLETICKER_H
