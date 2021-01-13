#ifndef WRITINGANIMATION_H
#define WRITINGANIMATION_H

#include "ticker.h"

#include <QLabel>
#include <QObject>
#include <QDebug>


/**
 * WritingAnimation
 *
 * WritingAnimation emits update() signal with a text, at
 * regular interval over a given duration.
 *
 * Each emit carries one more character than the previous
 * one. It starts with one character.
 *
 * The duration and the full text is given. The interval
 * between each update = duration/(length of text).
 *
 * If the updates are connected to the setText of a label,
 * it would look as if, the text being typed to the screen.
 */
class WritingAnimation : public QObject
{

    Q_OBJECT

public:
    // make writing animation given duration and text
    WritingAnimation(int duration = 1000, QString fullText="");

public slots:
    // start emiting updates
    void startWriting();

private slots:
    // emit update() with text
    // setup a write() if neccessary
    void write(int tickCount);

signals:
    // this signal tells listeners writing has started
    void started();

    // this signal gives out the next text update
    void update(QString nxt);

    // this signal tells listeners writing has finished
    void finished();

private:
    // the full text given at construction
    QString fullText;

    // ticker drives the writing
    //
    // everytime there is a tick, a signal carrying a
    // text is emitted
    //
    // the number of character increases with every emit
    Ticker ticker;
};

#endif // WRITINGANIMATION_H
