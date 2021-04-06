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

#include "settings.h"
#include "hostserver.h"
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

private:
    void createAudioInput();
    void createAudioOutput();
    inline qint16 applyMicVolumeToSample(qint16 iSample);
    inline qint16 applyPhonesVolumeToSample(qint16 iSample);
    void setNewDevice();

signals:
    void signalSendToPhones(qint16*, qint64&);

private slots:
    // Now we rad from mic and send to phons, bit later...
    void slotReadData();                // Read data from mic and send to UdpSocket
    void slotWriteData(qint16*, qint64&); // Write data from UdpSocket and send to phones
    void slotMenuAction(QAction* action);
    void slotChangeDevice();

    // Network slots
    void slotConnectToServer();

protected:
    void closeEvent(QCloseEvent* event);

private:
    // Device properties
    QAudioDeviceInfo m_inputDeviceInfo;
    QAudioDeviceInfo m_outputDeviceInfo;
    QScopedPointer<QAudioInput> m_audioInput;
    QScopedPointer<QAudioOutput> m_audioOutput;
    QScopedPointer<QIODevice> m_inputDevice;
    QIODevice* m_outputDevice; // I don't know why scoped pointer doesn't work
    QByteArray m_buffer;
    QAudioFormat m_format;
    qint32 m_maxAmplitude;

    // Settings window
    Settings* m_settings;
    // Host create window
    HostServer* m_hostServer;

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
