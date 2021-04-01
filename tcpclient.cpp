#include "tcpclient.h"

TcpClient::TcpClient(QWidget *parent) :
    QWidget(parent),
    nextBlockSize(0)
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
}

void TcpClient::slotReadyToRead()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_3);
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
        emit sendTextToChat(message);

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
    emit sendTextToChat(error);
}

void TcpClient::slotSendToServer()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << QTime::currentTime() << inputTextLine->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    inputTextLine->clear();
    tcpSocket->write(arrBlock);
}

void TcpClient::slotConnected()
{
    emit sendTextToChat("Connection established!");
}
