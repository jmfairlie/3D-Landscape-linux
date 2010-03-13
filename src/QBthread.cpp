#include "QBthread.h"

QBthread::QBthread(QBtooth *qBtooth, QObject *parent):
        QThread(parent)
{
    this->qBtooth = qBtooth;
}

void QBthread::run()
{
    QTextStream out(stdout);
    QThread::setTerminationEnabled();

    this->qBtooth->setPortParameters();

    if(this->qBtooth->openPort())
    {
        while(true)
        {
            if(!this->qBtooth->receiveData())
            {
                //out << "Thread was terminated!" << endl;
                this->qBtooth->closePort();
                return;
            }
        }
    }
    else
    {
        //out << "Thread was terminated, port not opened!" << endl;
        return;
    }

    exec();
}
