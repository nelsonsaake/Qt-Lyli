#include "writinganimation.h"



WritingAnimation::WritingAnimation(int duration, QString text)
    : fullText(text), ticker(duration, text.size())
{
    // connect()
    // whenever ticker ticks(i.e sends a ticked signal),
    // let this WritingAnimation write
    connect(&ticker,
            &Ticker::ticked,
            this,
            &WritingAnimation::write);
}

void WritingAnimation::startWriting()
{
    // starts ticker and
    // emits tell slots/listners that writing has started

    ticker.forward();
    emit started();
}

void WritingAnimation::write(int tickCount)
{
    // copy the first n characters from the fullText given,
    // and emit it

    // n = tickCount, tickCount start from zero so we can use
    // it, other wise we would need to maintain our own int
    // value

    QString txt;
    for(int i=0; i<tickCount && i<fullText.size(); i++){
        txt += fullText[i];
    }

    // emit update if txt is fullText
    // otherwise we are done
    if(txt!=fullText) emit update(txt);
    else {
        emit finished();
        ticker.stop();
    }
}
