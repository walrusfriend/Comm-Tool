#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QtWidgets>
#include <QHostInfo>

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget* parent = 0);

    void connectToHost(const QString& host, const int port);

signals:
    void signalSendTextToChat(QString);

private:
    QTcpSocket* tcpSocket;
    quint16 nextBlockSize;
    QHBoxLayout* hlayout;
    QLineEdit* inputTextLine;
    QPushButton* pbt_send;

    QString UserName;

private slots:
    void slotReadyToRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
};

#endif // TCPCLIENT_H
