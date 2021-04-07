#ifndef HOSTSERVER_H
#define HOSTSERVER_H

#include <QDialog>

#include "tcpserver.h"

namespace Ui {
class HostServer;
}

class HostServer : public QDialog
{
    Q_OBJECT

public:
    explicit HostServer(QWidget *parent = nullptr);
    ~HostServer();

    TcpServer* tcpServer;

signals:
    void signalSendTextToChat(QString);

public slots:
    void slotCreateTcpServer();

private:
    Ui::HostServer *ui;
};

#endif // HOSTSERVER_H
