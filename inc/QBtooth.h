#ifndef QBTOOTH_H
#define QBTOOTH_H

#include <QString>
#include <QTextStream>
#include <QThread>
#include <QStringList>
#include <QTime>
#include <qextserialport.h>

//Accelerometer messages types
#define ACCMSG 0x00
#define NVDMSG 0x01
#define NVUMSG 0x02
#define NVNMSG 0x03
#define NVCMSG 0x04

struct AccData
{
    quint32 msgType;
    qint32 x, y, z;
};

class QBtooth : public QObject
{
    Q_OBJECT

public:
    QBtooth(quint16 portNum);
    virtual ~QBtooth();
    //receives data from serial port
    bool receiveData();

public: //temporary public
    QextSerialPort *port;
	QTime *t;
    void setPortParameters();
    //opens serial port
    bool openPort();
    //previous time stamp (time of the previous data read)
    int prevTs;

public:
    //closes serial port
    void closePort();
    //Acc sensor message data
    AccData accData;

 signals:
     void dataReceived(AccData accData);
     void disconnected(bool disconnected);

};

#endif // QBTOOTH_H
