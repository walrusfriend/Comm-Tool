#include <cmath>

#include "mainwindow.h"
#include "ui_mainwindow.h"

// TO DO Implement data compession ot raw microphone data
// TO DO !!! Replace TCP protocol by UDP !!!
// Broadcast signals and many other feature

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    audio(new AudioInterface(this)),
    settings(new Settings(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Refresh the volume value
    connect(settings, SIGNAL(signalMicVolumeChanged(int)),
            audio, SLOT(slotMicVolumeChanged(int)));
    connect(settings, SIGNAL(signalPhonesVolumeChanged(int)),
            audio, SLOT(slotPhonesVolumeChanged(int)));

    connect(settings, SIGNAL(signalHearYourselfStateChanged(int)),
            audio, SLOT(slotHearYourselfStateChanged(int)));

    // Draw volume bars
    connect(audio, &AudioInterface::drawMicVolumeBar,
            settings, &Settings::drawMicVolume);
    connect(audio, &AudioInterface::drawPhonesVolumeBar,
            settings, &Settings::drawPhonesVolume);

    // Change device after selection in combo box
    connect(settings, &Settings::signalDeviceIsSelected, audio, &AudioInterface::slotChangeDevice);

    // Parse address line and check for correct ip
    QRegExp exp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}:[0-9]{1,5}");
    QValidator* val = new QRegExpValidator(exp, this);
    ui->le_address->setValidator(val);

    // TCP client
    tcpClient = new TcpClient;
    connect(tcpClient, SIGNAL(signalSendTextToChat(QString)),
            ui->te_chat, SLOT(append(QString)));

    // When the enter key is pressed, emit a signal like a "connect" button
    connect(ui->le_address, SIGNAL(returnPressed()), ui->pbt_connect, SIGNAL(clicked()));

    // Layout
    ui->verticalLayout->addWidget(tcpClient);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotConnectToServer()
{
    QStringList dict = ui->le_address->text().split(":");
    tcpClient->connectToHost(dict[0], dict[1].toInt());
}


void MainWindow::slotMenuAction(QAction* action)
{
    if (action->text() == "&Settings")
        settings->show();

    if (action->text() == "&Exit")
        qApp->exit();

    if (action->text() == "&About...")
        qDebug() << "We're in the \"about\" section!";
}

// if main widnow is closed, it closes all windows of the program
void MainWindow::closeEvent(QCloseEvent*)
{
    qApp->exit();
}
