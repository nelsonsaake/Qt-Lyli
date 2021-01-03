#ifndef CANCELCOOPERATIVE_H
#define CANCELCOOPERATIVE_H

#include <QObject>



class Worker : public QObject
{

    Q_OBJECT

public:
    Worker()
    {
        mIsCancelled = false;
    }
    ~Worker(){}


public slots:
    bool isCancelled(){
        return mIsCancelled;
    }

    void clearIsCancel(){
        mIsCancelled = false;
        emit clearCancel();
    }

    void setIsCancelled(){
        mIsCancelled = true;
        emit workCancelled();
    }

signals:
    void clearCancel();
    void workCancelled();

private:
    bool mIsCancelled;
};

#endif // CANCELCOOPERATIVE_H
