#include "writinganimation.h"



WritingAnimation::WritingAnimation(int duration, QString text)
    : text(text), ticker(duration, text.size())
{
    connect(&ticker,
            &Ticker::ticked,
            this,
            &WritingAnimation::writeChar);
}

void WritingAnimation::startWriting(){
    ticker.forward();
    emit started();
}

void WritingAnimation::writeChar(int tickCount){

    QString txt;
    for(int i=0; i<tickCount; i++){
        txt += text[i];
    }

    if(tickCount<=text.size()) emit update(txt);
    else emit finished();
}
