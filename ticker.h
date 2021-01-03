#ifndef SINGLETICKER_H
#define SINGLETICKER_H

#include "worker.h"

#include <QObject>
#include <QTimer>



class Ticker: public QObject
{
    Q_OBJECT
public:
    Ticker(int duration=100, int value=1, int delay=0)
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

    void setDuration(int value)
    {
        duration = value;
    }

    void setValue(int value)
    {
        this->value = value;
    }

    void setTickInterval(int value){
        if(foreverModeTickInterval<100) return;
        foreverModeTickInterval = value;
    }

protected:
    int tickInterval(){
        if(duration==0 || value==0) return 0;
        return duration/value;
    }

    void startTicking(){
        // the first tick will increment the value
        // and emit it
        // => tick count will start at '0'
        tickCount = -1;

        if(delay>0) timer.start(delay);
        else tick();
    }

private slots:
    void tick(){
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

public slots:
    void forward(){
        if(duration==0||value==0) return;
        foreverTickMode = false;
        startTicking();
    }

    void start(){
        foreverTickMode = true;
        foreverTickStopped = false;
        startTicking();
    }

    void stop(){
        foreverTickStopped = true;
    }

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
