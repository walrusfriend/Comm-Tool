#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include <QtWidgets/QMessageBox>
#include <QDebug>
#include <QScopedPointer>

#include "audiointerface.h"
#include "settings.h"
#include "tcpclient.h"
//#include "udpserver.h"
//#include "udpclient.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend Settings;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotMenuAction(QAction* action);

    // Network slots
    void slotConnectToServer();

protected:
    void closeEvent(QCloseEvent* event);

private:
    // Audio device settings
    AudioInterface* audio;

    // Settings window
    Settings* settings;

    Ui::MainWindow *ui;

    // Network members
    // Will use peer-to-peer combined network
    // TCP to chat and various confirmations
//    TcpServer* tcpServer;
    TcpClient* tcpClient;

    // TO DO
    // UDP to data from microphone
    //UdpServer* udpServer;
    //UdpCliend* udpCliend;

};

#endif // MAINWINDOW_H
