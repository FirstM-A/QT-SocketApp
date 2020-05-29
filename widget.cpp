#include "widget.h"
#include "ui_widget.h"
#include <QTcpSocket>
#include <QString>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //1、初始化
    server = new QTcpServer(this);
    ui->Button_openServer->setEnabled(true);
    ui->Button_sendMsg->setEnabled(false);
    ui->Button_closeServer->setEnabled(false);

    connect(server,&QTcpServer::newConnection,this,&Widget::newconnectionSlot);//新连接处理

}

Widget::~Widget()
{
    delete ui;
}

void Widget::newconnectionSlot()
{
    QTcpSocket *_socket = server->nextPendingConnection();
    client.append(_socket);//将连接成功的套接字存放到Qlist中
    QString _ip = _socket->peerAddress().toString();
    quint16 _port = _socket->peerPort();
    ui->plainTextEdit_Rec->appendPlainText(QString("[%1:%2] 客户端连接成功！").arg(_ip).arg(_port));
    //为每个套接字的来消息信号绑定槽函数
    connect(_socket,&QTcpSocket::readyRead,this,&Widget::readDataSlot);//来消息信号
    connect(_socket,&QTcpSocket::disconnected,this,&Widget::disconnectionSlot);//断开连接信号
}
void Widget::readDataSlot()
{
    //由于不知道对应着哪一个socket，所以遍历每个套接字，当发现有数据，就把它显示出来
    for(int i = 0;i<client.size();i++)
    {
        QByteArray _buff = client[i]->readAll();
        if(true == _buff.isNull())
            continue;
        QString _ip = client[i]->peerAddress().toString();
        quint16 _port = client[i]->peerPort();
        ui->plainTextEdit_Rec->appendPlainText(QString("[%1:%2]").arg(_ip).arg(_port));
        ui->plainTextEdit_Rec->appendPlainText(QString(_buff));
//        ui->plainTextEdit_Rec->appendPlainText(QString("\n"));
    }
}
//“打开服务器”按键槽函数
void Widget::on_Button_openServer_clicked()
{
    server->listen(QHostAddress::Any,ui->lineEdit_Port->text().toUShort());//开启监听
    ui->Button_openServer->setEnabled(false);
    ui->Button_sendMsg->setEnabled(true);
    ui->Button_closeServer->setEnabled(true);
    ui->lineEdit_IP->setText(server->serverAddress().toString());
    ui->lineEdit_Port->setText(QString::number(server->serverPort()));
}
//“关闭服务器”按键槽函数
void Widget::on_Button_closeServer_clicked()
{
    //1、停止监听
    server->close();
    //2、主动和每一个已连接的客户端断开
    for(int i = 0;i< client.size();i++)
    {
        client[i]->disconnectFromHost();
    }
    ui->Button_openServer->setEnabled(true);
    ui->Button_sendMsg->setEnabled(false);
    ui->Button_closeServer->setEnabled(false);
}
//断开连接信号
void Widget::disconnectionSlot()
{
    for(int i = 0;i<client.size();i++)
    {
        if(QTcpSocket::UnconnectedState == client[i]->state())
        {
            QString _ip = client[i]->peerAddress().toString();
            quint16 _port = client[i]->peerPort();
            ui->plainTextEdit_Rec->appendPlainText(QString("[%1:%2] 已断开连接！").arg(_ip).arg(_port));
            //移除该套接字
            client.removeAt(i);
            break;
        }
    }
}
//发送消息按键
void Widget::on_Button_sendMsg_clicked()
{
    //群发
    for(int i = 0;i<client.size();i++)
    {
        QString data = ui->plainTextEdit_Send->toPlainText();
        qint64 ok = client[i]->write(data.toUtf8());
        if(ok == -1)
        {
            qDebug() << "消息发送失败" << endl;
        }
    }
}
