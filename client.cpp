#include "client.h"
#include "ui_client.h"


Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    client = new QTcpSocket(this);
    ui->pushButton_Send->setEnabled(false);
    ui->pushButton_connect->setEnabled(true);
    ui->pushButton_disconnect->setEnabled(false);

    connect(client,&QTcpSocket::connected,this,&Client::connectSuccessSlot);
    connect(client,&QTcpSocket::disconnected,this,&Client::disconnectSlot);
    connect(client,&QTcpSocket::readyRead,this,&Client::newDataSlot);

}

Client::~Client()
{
    delete ui;
}
void Client::connectSuccessSlot()
{
    ui->plainTextEdit_Rec->appendPlainText(QString("成功连接服务器!"));
    ui->pushButton_Send->setEnabled(true);
    ui->pushButton_connect->setEnabled(false);
    ui->pushButton_disconnect->setEnabled(true);
}
void Client::disconnectSlot()
{
    ui->plainTextEdit_Rec->appendPlainText(QString("与服务器断开连接!"));
    ui->pushButton_Send->setEnabled(false);
    ui->pushButton_connect->setEnabled(true);
    ui->pushButton_disconnect->setEnabled(false);

}
void Client::newDataSlot()
{
    QByteArray _data = client->readAll();
    ui->plainTextEdit_Rec->appendPlainText(QString(_data));
//    ui->plainTextEdit_Rec->appendPlainText(QString("\n"));
}
void Client::on_pushButton_connect_clicked()
{
    QString _ip = ui->lineEdit_IP->text();
    quint16 _port = ui->lineEdit_Port->text().toUInt();
    client->connectToHost(_ip,_port);


}

void Client::on_pushButton_disconnect_clicked()
{
    client->disconnectFromHost();

}

void Client::on_pushButton_Send_clicked()
{
    client->write(ui->plainTextEdit_Send->toPlainText().toUtf8());
}
