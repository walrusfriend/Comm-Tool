#include "hostserver.h"
#include "ui_hostserver.h"

HostServer::HostServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HostServer)
{
    ui->setupUi(this);

    // Create regular expression to set validator for correct port
    QRegExp exp("[0-9]{1,5}");
    QValidator* val = new QRegExpValidator(exp, this);
    ui->le_port->setValidator(val);

    // When the button is clicked, a server is created
    connect(ui->pbt_create, SIGNAL(clicked()), SLOT(slotCreateTcpServer()));
}

HostServer::~HostServer()
{
    delete ui;
}

void HostServer::slotCreateTcpServer()
{
    // TO DO Able to close tcp server
    qint16 port = ui->le_port->text().toInt();

    // createTCP server
    tcpServer = new TcpServer(port, this);

    // Connect server with chat via a signal
    connect(tcpServer, SIGNAL(signalSendTextToChat(QString)), SIGNAL(signalSendTextToChat(QString)));

    if (tcpServer->isListening())
        emit signalSendTextToChat("Server created!");
}
