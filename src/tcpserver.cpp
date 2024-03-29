#include "tcpserver.h"

TcpServer::TcpServer(int port, QWidget* parent) :
    QWidget(parent),
    m_nextBlockSize(0),
    serverName(QHostInfo::localHostName())
{
    m_tcpServer = new QTcpServer(this);
    // Close server if error sutiation is occured
    if (!m_tcpServer->listen(QHostAddress::Any, port)) {
        QMessageBox::critical(0, "Server error",
                              "Unable to start the server:" + m_tcpServer->errorString());
        m_tcpServer->close();
        return;
    }

    // Write start message to the chat
    emit signalSendTextToChat("Server created");

    connect(m_tcpServer, SIGNAL(newConnection()),
            this, SLOT(slotNewConnection()));
}

void TcpServer::slotNewConnection()
{
    QTcpSocket* clientSocket = m_tcpServer->nextPendingConnection();
    connect(clientSocket, SIGNAL(disconnected()),
            clientSocket, SLOT(deleteLater()));

    connect(clientSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient()));

    sendToClient(clientSocket, "Connected!");
}

void TcpServer::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*) sender();
    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_5_15);
    for(;;) {
        if (!m_nextBlockSize) {
            if (clientSocket->bytesAvailable() < sizeof(quint16))
                break;
        }
        in >> m_nextBlockSize;

        if (clientSocket->bytesAvailable() < m_nextBlockSize)
            break;

        QTime time;
        QString str;
        in >> time >> str;

        QString message = time.toString() + " " + "Client: " + str;

        // write message to chat
        emit signalSendTextToChat(message);

        m_nextBlockSize = 0;

        // Send to the client that the server read a message
        //sendToClient(clientSocket, serverName + ": " + str);
    }
}

void TcpServer::sendToClient(QTcpSocket *socket, const QString &str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    socket->write(arrBlock);
}



