#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_Button_openServer_clicked();

    void on_Button_closeServer_clicked();

    void on_Button_sendMsg_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QList<QTcpSocket*> client;

private:
    void newconnectionSlot();
    void readDataSlot();
    void disconnectionSlot();


};

#endif // WIDGET_H
