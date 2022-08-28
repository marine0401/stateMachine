#include <QValidator>

//#include "qrencode-4.1.0/qrencode.h"
#include "widget.h"
#include "ui_widget.h"
#include "mqttreceiver.h"
#include "tools.h"
#include <QJsonObject>
#include <QPainter>
#include <QHostInfo>
#include <QJsonDocument>

extern "C"{
    #include "qrencode-4.1.0/qrencode.h"
}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

   ui->setupUi(this);
   m_widgetDevice = new MessageDevice(this);
   connect(m_widgetDevice, &MessageDevice::sendMessage, this,  &Widget::sendMessage);
   connect(this, &Widget::sendMessage, this, &Widget::onHandleMessage);

   QIntValidator value(0, 1000000, this);
   ui->interval->setValidator(&value);
   led = new LedController(ui, this);

   MQTTReceiver *m_MQTTReceiver = new MQTTReceiver();
   m_MQTTReceiver->moveToThread(&m_MQTTReceiverThread);
   connect(&m_MQTTReceiverThread, &QThread::finished, m_MQTTReceiver, &QObject::deleteLater);
   m_MQTTReceiverThread.start();
   m_MQTTReceiver->init("www.beatingcai.com", "test", "public");
   //该信号发出之后，m_MQTTReceiver的成员函数是在 m_MQTTReceiverThread 运行
   emit m_MQTTReceiver->start();

    // HandleMessage 运行在其他线程
    HandleMessage *messageCenter = new HandleMessage();
    messageCenter->moveToThread(&m_handleMessageThread);
    connect(&m_handleMessageThread, &QThread::finished, messageCenter, &QObject::deleteLater);
    m_handleMessageThread.start();

    connect(m_MQTTReceiver, &MQTTReceiver::dispatchMessage, messageCenter, &HandleMessage::onReceiveMessage);
    connect(this, &Widget::dispatchMessage, messageCenter, &HandleMessage::onReceiveMessage);
    messageCenter->registerMessageListen("led", (MessageDevice *)led);
    messageCenter->registerMessageInput("mqtt", (MessageDevice *)m_MQTTReceiver);
    messageCenter->registerMessageInput("widget", (MessageDevice *)this);


    connect(led, &LedController::messageReponse, messageCenter, &HandleMessage::onReceiveMessageResponse);
    connect(m_MQTTReceiver, &MQTTReceiver::myState, led, &LedController::onListenNetState);

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++czh+++++++++++++++++++++++++++++++++++++++++++*/


    m_server = new MyServer();
    m_server->listen(QHostAddress::Any,8080);

    //initialize thread list

    qDebug() << "Main Thread id" << QThread::currentThreadId() << endl;

    ui->codeWidget->setVisible(false);

}



Widget::~Widget()
{
    m_handleMessageThread.quit();
    m_handleMessageThread.wait();
    m_MQTTReceiverThread.quit();
    m_MQTTReceiverThread.wait();

    delete m_server;
    delete ui;
}



void Widget::on_Light_clicked()
{
    QJsonObject msg{
        {"msgId",100},
        {"operation", 0},
        {"destSubDevice", "led"},
        {"arg",0},
        {"payload", 500}
    };
    msg.insert("MessageType", "widget");

    emit this->dispatchMessage(msg);
}

void Widget::on_Blink_clicked()
{

    bool ok = false;
    QString intervalString = ui->interval->text();
    int interval = intervalString.toInt(&ok, 10);
    if(ok){
        QJsonObject msg{
            {"msgId",100},
            {"operation", 0},
            {"destSubDevice", "led"},
            {"arg",2},
            {"payload", interval}
        };
        msg.insert("MessageType", "widget");

        emit this->dispatchMessage(msg);
    }

}

void Widget::on_Close_clicked()
{
    QJsonObject msg{
        {"msgId",100},
        {"operation", 0},
        {"destSubDevice", "led"},
        {"arg",1},
        {"payload", 500}
    };
    msg.insert("MessageType", "widget");

    emit this->dispatchMessage(msg);
}

void Widget::onHandleMessage(const QJsonObject &msg)
{
    qDebug() << msg;

}

void Widget::on_dispalyCode_clicked()
{
    QJsonObject msg{
        {"deviceName",QHostInfo::localHostName()},
        {"deviceId", Tools::getCpuId()},
        {"deviceType", "led"}
    };
    GenerateQRcode(QString(QJsonDocument(msg).toJson()));
    if(ui->codeWidget->isVisible())
    {
       ui->dispalyCode->setText("点击扫码控制");
       ui->codeWidget->setVisible(false);

    }else
    {
        ui->dispalyCode->setText("开始控制");
        ui->codeWidget->setVisible(true);

    }

}

void Widget::GenerateQRcode(QString tempstr)
{
    QRcode *qrcode; //二维码数据
    //QR_ECLEVEL_Q 容错等级
    qrcode = QRcode_encodeString(tempstr.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
    qint32 temp_width=ui->code->width(); //二维码图片的大小
    qint32 temp_height=ui->code->height();
    qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
    double scale_x = (double)temp_width / (double)qrcode_width; //二维码图片的缩放比例
    double scale_y =(double) temp_height /(double) qrcode_width;
    QImage mainimg=QImage(temp_width,temp_height,QImage::Format_ARGB32);
    QPainter painter(&mainimg);
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, temp_width, temp_height);
    QColor foreground(Qt::black);
    painter.setBrush(foreground);
    for( qint32 y = 0; y < qrcode_width; y ++)
    {
        for(qint32 x = 0; x < qrcode_width; x++)
        {
            unsigned char b = qrcode->data[y * qrcode_width + x];
            if(b & 0x01)
            {
                QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                painter.drawRects(&r, 1);
            }
        }
    }
    QPixmap mainmap=QPixmap::fromImage(mainimg);
    ui->code->setPixmap(mainmap);
    ui->code->setVisible(true);
}
