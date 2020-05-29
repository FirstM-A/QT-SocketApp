#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void on_pushButton_connect_clicked();

    void on_pushButton_disconnect_clicked();

    void on_pushButton_Send_clicked();

    void connectSuccessSlot();
    void disconnectSlot();
    void newDataSlot();

private:
    Ui::Client *ui;
    QTcpSocket *client;
};

#endif // CLIENT_H
