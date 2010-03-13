#ifndef QBTHREAD_H
#define QBTHREAD_H

#include <QThread>
#include <QBtooth.h>
#include <QTextStream>

class QBthread : public QThread
{
private:
    //pointer to QBtooth class
    QBtooth *qBtooth;

protected:
    virtual void run();

public:
    QBthread(QBtooth *qBtooth, QObject *parent = 0);


};


#endif // QBTHREAD_H
