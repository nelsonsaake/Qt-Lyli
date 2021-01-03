#ifndef WRITINGANIMATION_H
#define WRITINGANIMATION_H

#include "ticker.h"

#include <QLabel>
#include <QObject>
#include <QDebug>

class WritingAnimation : public QObject
{
    Q_OBJECT
public:
    WritingAnimation(int duration = 1000, QString text="")
        : text(text), ticker(duration, text.size())
    {
        connect(&ticker,
                &Ticker::ticked,
                this,
                &WritingAnimation::writeChar);
    }

public slots:
    void startWriting(){
        ticker.forward();
        emit started();
    }

private slots:
    void writeChar(int tickCount){

        QString txt;
        for(int i=0; i<tickCount; i++){
            txt += text[i];
        }

        if(tickCount<=text.size()) emit update(txt);
        else emit finished();
    }

signals:
    void started();
    void update(QString nxt);
    void finished();

private:
    QString text;
    Ticker ticker;
};

#endif // WRITINGANIMATION_H
