#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QtNetwork>
#include <QtWidgets>

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    TcpServer(int port, QWidget* parent = 0);

public slots:
    virtual void slotNewConnection();
    void slotReadClient();

signals:
    void sendTextToChat(QString str);

private:
    QTcpServer* m_tcpServer;
    QTextEdit* m_txt;
    quint16 m_nextBlockSize;

private:
    void sendToClient(QTcpSocket* socket, const QString &str);
};

#endif // TCPSERVER_H
