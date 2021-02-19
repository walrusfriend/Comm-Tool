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
    int ApplyVolumeToSample(qint16 iSample);
    void setNewDevice();

private slots:
    void readMore();
    void sliderValueChanged(int value);
    void menuAction(QAction* action);
    void changeDevice();

private:
    QAudioDeviceInfo m_inputDeviceInfo;
    QAudioDeviceInfo m_outputDeviceInfo;
    QScopedPointer<QAudioInput> m_audioInput;
    QScopedPointer<QAudioOutput> m_audioOutput;
    QScopedPointer<QIODevice> m_inputDevice;
//    QScopedPointer<QIODevice> m_outputDevice;  // Something went wrong with scoped ponter
    QIODevice*  m_outputDevice;
    QByteArray m_buffer;
    QAudioFormat m_format;
    int m_volume = 0;
    Settings* m_settings;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
