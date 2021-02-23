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
    qint16 ApplyVolumeToSample(qint16 iSample);
    void setNewDevice();

private slots:
    void readMore();
    void sliderValueChanged(int value);
    void menuAction(QAction* action);
    void changeDevice();

private:
    // Device properties
    QAudioDeviceInfo m_inputDeviceInfo;
    QAudioDeviceInfo m_outputDeviceInfo;
    QScopedPointer<QAudioInput> m_audioInput;
    QScopedPointer<QAudioOutput> m_audioOutput;
    QScopedPointer<QIODevice> m_inputDevice;
    QIODevice*  m_outputDevice;
    QByteArray m_buffer;
    QAudioFormat m_format;
    qint32 m_maxAmplitude;

    int m_micVolume;
    int m_phnVolume;

    // Pointer to settings window
    Settings* m_settings;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
