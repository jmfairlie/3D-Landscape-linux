#include <QBtooth.h>

QBtooth::QBtooth(quint16 portNum)
{
    QString portName("");
    portName.setNum(portNum);
	
	this->t = new QTime();
    this->t->start();
	this->prevTs = this->t->elapsed();


    if(portNum < 10)
    {
        portName.prepend("COM");
        this->port = new QextSerialPort(portName, QextSerialPort::EventDriven);
    }
    else if(portNum >= 10)
    {
        portName.prepend("\\\\.\\COM");
        this->port = new QextSerialPort(portName, QextSerialPort::EventDriven);
    }
}

void QBtooth::setPortParameters()
{
    this->port->setBaudRate(BAUD9600);
    this->port->setFlowControl(FLOW_OFF);
    this->port->setParity(PAR_NONE);
    this->port->setDataBits(DATA_8);
    this->port->setStopBits(STOP_1);
}

bool QBtooth::openPort()
{
    QTextStream out(stdout);

    if(this->port->isOpen())
        this->closePort();

    /*bool x = */this->port->open(QIODevice::ReadOnly);

    //out << "Port is opened: " << x << "\n";
    
    if(!this->port->isOpen())
    {
       //out << "error: port was not opened" << endl;
       Sleep(1000);
       emit this->disconnected(true);
       return false;
    }

    return true;
}

//returns false, if device was disconnected
bool QBtooth::receiveData()
{
    QTextStream out(stdout);

    char data[1024];
    ZeroMemory(data, 1024);

    //initial timestamp used to assess time needed to receive 27 bytes of data from the port
    int portTimeoutTs = t->elapsed();

    //27 bytes - size of the message
    while(port->bytesAvailable() < 27)
    {
        //disconnected
        if(this->t->elapsed() - portTimeoutTs > 2000)
        {
                   //out << "error: port was closed" << endl;
                   Sleep(1000);
                   emit this->disconnected(true);
                   return false;
        }
    }

    int bytesRead = port->readLine(data, 1024);

    data[bytesRead] = '\0';

    //out << "data read; " << data << endl;
    //string used to parse data
    QString dataStr(data);

    if(dataStr.startsWith("$NVD"))
    {
        this->accData.msgType = NVDMSG;
        emit this->dataReceived(this->accData);
        return true;
    }
    else if(dataStr.startsWith("$NVU"))
    {
        this->accData.msgType = NVUMSG;
        emit this->dataReceived(this->accData);
        return true;
    }
    else if(dataStr.startsWith("$NVC"))
    {
        this->accData.msgType = NVCMSG;
        emit this->dataReceived(this->accData);
        return true;
    }

    //we need to use data only each 42 milliseconds
    //we read data, even if we don't use it, just to clean the receive buffer
    if(this->t->elapsed() - this->prevTs > 42)
    {
        if((dataStr.length() != 0) && (bytesRead > 0))
        {
            QStringList dataList = dataStr.split(",");

            //ACC packet
            if(dataList[0] == QString("$ACC"))
            {
                if(dataList.size() < 5)
                {
                    //out << "Accelerator packet error!!!" << endl;
                }
                else
                {
                    /*out << "X: " << dataList[1] << endl;
                    out << "Y: " << dataList[2] << endl;
                    out << "Z: " << dataList[3] << endl;*/
                    this->accData.msgType = ACCMSG;

                    bool convOKx = true;
                    bool convOKy = true;
                    bool convOKz = true;

                    this->accData.x = dataList[1].toInt(&convOKx, 10);

                    this->accData.y = dataList[2].toInt(&convOKy, 10);

                    this->accData.z = dataList[3].toInt(&convOKz, 10);

                    if(convOKx && convOKy && convOKz)
                    {
                        //emit data receive signal to process by Engine3D
                        emit this->dataReceived(this->accData);
                    }
                }
             }             
        }

        //update timestamp of the previous data usage
        this->prevTs = t->elapsed();
    }
	
    return true;
}

void QBtooth::closePort()
{
    this->port->close();
}

QBtooth::~QBtooth()
{
    delete this->port;
	delete this->t;
}

