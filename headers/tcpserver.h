#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QtNetwork>
#include <QtWidgets>
#include <QHostAddress>

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    TcpServer(int port, QWidget* parent = 0);

    bool isListening() { return m_tcpServer->isListening(); }
    QHostAddress serverAddress() { return m_tcpServer->serverAddress(); }
    qint16 serverPort() { return m_tcpServer->serverPort(); }

public slots:
    virtual void slotNewConnection();
    void slotReadClient();

signals:
    void signalSendTextToChat(QString str);

private:
    QTcpServer* m_tcpServer;
    QTextEdit* m_txt;
    quint16 m_nextBlockSize;

    QString serverName;

private:
    void sendToClient(QTcpSocket* socket, const QString &str);
};

#endif // TCPSERVER_H
