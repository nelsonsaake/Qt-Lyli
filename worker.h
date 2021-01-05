#ifndef CANCELCOOPERATIVE_H
#define CANCELCOOPERATIVE_H

#include <QObject>



class Worker : public QObject
{

    Q_OBJECT

public:
    Worker();
    ~Worker();


public slots:
    bool isCancelled();

    void clearIsCancel();

    void setIsCancelled();

signals:
    void clearCancel();
    void workCancelled();

private:
    bool mIsCancelled;
};

#endif // CANCELCOOPERATIVE_H
