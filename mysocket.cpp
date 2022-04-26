#include "mysocket.h"

MySocket::MySocket(qintptr handle):QObject(),m_socketDescriptor(handle)
{
    send_buffer.append(0x57);

    send_buffer.append(0x01);
    send_buffer.append(0x01);
    send_buffer.append(0x01);
    send_buffer.reserve(4);
}
MySocket::~MySocket()
{
    delete m_socket;
}
void MySocket::init()
{
    m_socket = new QTcpSocket();
    m_socket->setSocketDescriptor(m_socketDescriptor);

    //m_socket->write("connect success\n");
    qDebug() << "********************************init thread ID"<< QThread::currentThreadId() << endl;
    connect(m_socket,&QTcpSocket::disconnected,[=](){
        emit Disconnected();
        qDebug() << "********************************socket delete " <<endl;
        this->deleteLater();
    });
    connect(m_socket,&QTcpSocket::readyRead,this,&MySocket::read,Qt::DirectConnection);
}
void MySocket::read()
{
    qDebug() << "***********************read thread ID"<< QThread::currentThreadId() << endl;
    m_buffer.append(m_socket->readAll());

    char * dataPtr = m_buffer.data();
    Packet *packetPtr = (Packet *)dataPtr;
    qDebug() << "m_buffer size" << m_buffer.size() << endl;
    qDebug() <<"sync"<< packetPtr->sync<<endl;
    qDebug() <<"length"<< packetPtr->length<<endl;
    qDebug() <<"cmdID"<< packetPtr->cmdID<<endl;
    if(m_buffer.size() > 3)
    {
        if(packetPtr->sync == SYNC)
        {
            if(m_buffer.size() >= packetPtr->length)
            {

                if(packetPtr->cmdID == 0x01)
                {
                    qint64 dataLen = packetPtr->length - 4; //数据长度
                    char * data = (char *)malloc(dataLen);
                    memcpy(data,dataPtr+4,dataLen);
                    send_buffer[3] = packetPtr->cmdID;

                    send_buffer.append(data[0]);
                    if(data[0] & 0x01)
                    {

                        QFile file("/sys/class/net/eth1/address");
                        file.open(QIODevice::ReadOnly);

                        send_buffer.append(file.read(17));

                    }

                    send_buffer[2] = ((unsigned short)send_buffer.size() >> 8) & 0xff;
                    send_buffer[1] = (unsigned short)send_buffer.size() & 0xff;


                    char *dat = send_buffer.data();
                       int j =0;

                        while (j < send_buffer.size()) {
                            qDebug() << " [" << QByteArray(1,*dat).toHex() << "] " ;
                            ++dat;
                            ++j;
                      }


                    m_socket->write(send_buffer);
                    m_socket->waitForBytesWritten();
                    send_buffer.remove(4,send_buffer.size() - 4);
                    delete data;
                    m_buffer.remove(0,packetPtr->length);

                }



             }
        }

    }





}

