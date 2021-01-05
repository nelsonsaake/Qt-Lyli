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
    WritingAnimation(int duration = 1000, QString text="");

public slots:
    void startWriting();

private slots:
    void writeChar(int tickCount);

signals:
    void started();
    void update(QString nxt);
    void finished();

private:
    QString text;
    Ticker ticker;
};

#endif // WRITINGANIMATION_H
