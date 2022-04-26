#ifndef MYSERVER_H
#define MYSERVER_H
#include<QTcpServer>
#include<QThread>
#include"mysocket.h"
class MyServer :public QTcpServer
{
public:
    MyServer();
    ~MyServer();

protected:
    virtual void incomingConnection(qintptr handle);

private:
    QList<QThread *> thread_List;

    QThread * getThread();
};

#endif // MYSERVER_H
