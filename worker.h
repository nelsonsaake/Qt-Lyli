#ifndef CANCELCOOPERATIVE_H
#define CANCELCOOPERATIVE_H

#include <QObject>


/**
 * Worker
 *
 * Worker allow long runnig class to do some mundane operation
 * such as cancellation
 *
 * For now, Worker only allows cancellation
 *
 * Cancelling here is co-operative: which means when cancel is
 * called, we done just cancel, we set the isCancel to true,
 * the class using(composition/inheritance) Worker will check
 * isCancel and handle it.
 */
class Worker : public QObject
{

    Q_OBJECT

public:
    //
    Worker();
    ~Worker();


public slots:
    // check is cancel state
    bool isCancelled();

    // isCancel = false
    void clearIsCancel();

    // isCancel = true
    void setIsCancelled();

signals:
    // tell listeners cancel is clear
    void clearCancel();

    // tell listeners worker is cancelled
    void workCancelled();

private:
    //
    bool mIsCancelled;
};

#endif // CANCELCOOPERATIVE_H
