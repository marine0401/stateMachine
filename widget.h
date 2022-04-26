#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QList>
#include<QTcpServer>
#include "ledcontroller.h"
#include "handlemessage.h"

#include"myserver.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

signals:
    void sendMessage(const QJsonObject &msg) const;
    void dispatchMessage(const QJsonObject &msg);


private slots:

    void on_Light_clicked();

    void on_Blink_clicked();

    void on_Close_clicked();

    void onHandleMessage(const QJsonObject &msg);



private:
    Ui::Widget *ui;
    LedController *led;
    QThread m_handleMessageThread;
    QThread m_MQTTReceiverThread;
    MessageDevice * m_widgetDevice = nullptr;


    QThread m_socketThread;
    MyServer *m_server;

};
#endif // WIDGET_H
