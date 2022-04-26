#ifndef MYSOCKET_H
#define MYSOCKET_H
#include<QObject>
#include<QTcpSocket>
#include<QTcpServer>
#include<QThread>
#include<QFile>
#include"protocol.h"
class MySocket:public QObject
{
    Q_OBJECT
public:
    MySocket(qintptr handle);
    ~MySocket();
    void init();
public slots:
    void read();
signals:
    void Disconnected();
private:
    QTcpSocket *m_socket;
    QTcpServer *m_server;
    QByteArray m_buffer;

    QByteArray send_buffer;

    qintptr m_socketDescriptor;

};

#endif // MYSOCKET_H
