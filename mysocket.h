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
    typedef void (*Handler)(const QByteArray &) ;
    MySocket(qintptr handle);
    ~MySocket();
    void init();
public slots:
    void read();
signals:
    void Disconnected();
private:
    static QTcpSocket *m_socket; // 暂时改为静态
    QTcpServer *m_server;
    QByteArray m_buffer;

    QByteArray send_buffer;

    qintptr m_socketDescriptor;

    QHash<eCmdID, Handler> m_handlers;
private slots:
    static void retMessage(const QByteArray &);

};

#endif // MYSOCKET_H
