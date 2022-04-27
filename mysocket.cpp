#include "mysocket.h"
#include "tools.h"

QTcpSocket *MySocket::m_socket;
MySocket::MySocket(qintptr handle):QObject(),m_socketDescriptor(handle)
{
    send_buffer.append(0x57);

    send_buffer.append(0x01);
    send_buffer.append(0x01);
    send_buffer.append(0x01);
    send_buffer.reserve(4);

    m_handlers.insert(GET_MESSAGE, retMessage);
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
                if(m_handlers.keys().contains((eCmdID)packetPtr->cmdID))
                {
                   Handler handler = m_handlers.value((eCmdID)packetPtr->cmdID);
                   QByteArray packet(dataPtr,packetPtr->length);
                   (*handler)(packet);
                }
                    m_buffer.remove(0,packetPtr->length);

             }
        }
    }

}

/**
 * @brief retMacAddress 返回mac地址
 */
void MySocket::retMessage(const QByteArray &data)
{
    QByteArray send_buffer;
    Packet *packetPtr = (Packet *)data.data();
    if(packetPtr->cmdID == GET_MESSAGE)
    {


        if(packetPtr->data[0] & MAC_ADDRESS)
        {
        #ifdef _WIN32
        //在win下编译
            send_buffer.append(Tools::getCpuId());
        #else

            QFile file("/sys/class/net/eth1/address");
            file.open(QIODevice::ReadOnly);

            send_buffer.append(file.read(17));

            file.close();
          #endif
        }

        // packetPtr->data[0] 发还客户端
        QByteArray sendPacket = commonPacket(GET_MESSAGE,packetPtr->data[0] ,send_buffer);

        m_socket->write(sendPacket);
        m_socket->waitForBytesWritten();
    }
}





