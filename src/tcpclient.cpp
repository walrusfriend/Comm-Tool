#include "tcpclient.h"

TcpClient::TcpClient(QWidget *parent) :
    QWidget(parent),
    nextBlockSize(0),
    UserName(QHostInfo::localHostName())
{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(tcpSocket, SIGNAL(readyRead()), SLOT(slotReadyToRead()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,      SLOT(slotError(QAbstractSocket::SocketError)));

    pbt_send = new QPushButton("&Send");
    connect(pbt_send, SIGNAL(clicked()), SLOT(slotSendToServer()));

    inputTextLine = new QLineEdit;
    // Emit a signal when the "enter" key is pressed
    connect(inputTextLine, SIGNAL(returnPressed()), SLOT(slotSendToServer()));

    // Layout
    hlayout = new QHBoxLayout;
    hlayout->addWidget(inputTextLine);
    hlayout->addWidget(pbt_send);

    setLayout(hlayout);
}

void TcpClient::connectToHost(const QString &host, const int port)
{
    tcpSocket->connectToHost(host, port);

    // If connection establish set focus on chat line
    if (tcpSocket->state() == QAbstractSocket::ConnectedState ||
        tcpSocket->state() == QAbstractSocket::ConnectingState)
    {
        inputTextLine->setFocus();

        // Send a welcome message to the server
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_15);
        out << quint16(0) << QTime::currentTime() << QString(UserName + " is connected!");

        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        tcpSocket->write(arrBlock);
    }

}

void TcpClient::slotReadyToRead()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_15);
    for (;;) {
        if (!nextBlockSize) {
            if (tcpSocket->bytesAvailable() < sizeof(quint16))
                break;
            in >> nextBlockSize;
        }

        if (tcpSocket->bytesAvailable() < nextBlockSize)
            break;

        QTime time;
        QString str;
        in >> time >> str;

        // write to chat
        QString message = time.toString() + " " + str;
        emit signalSendTextToChat(message);

        nextBlockSize = 0;
    }
}

void TcpClient::slotError(QAbstractSocket::SocketError err)
{
    QString error = "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                                 "The host was not found." :
                                 err == QAbstractSocket::RemoteHostClosedError ?
                                 "The remote host is closed." :
                                 err == QAbstractSocket::ConnectionRefusedError ?
                                 "The connection was refused." :
                                 QString(tcpSocket->errorString()));
    emit signalSendTextToChat(error);
}

void TcpClient::slotSendToServer()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QTime::currentTime() << inputTextLine->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    if(tcpSocket->write(arrBlock) == -1)
        emit signalSendTextToChat("Connect to server first!");
    else
        emit signalSendTextToChat(QTime::currentTime().toString() + ' ' +
                                  UserName + ": " +
                                  inputTextLine->text());
    inputTextLine->clear();
}

void TcpClient::slotConnected()
{
    emit signalSendTextToChat("Connection established!");
}
