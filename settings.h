#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QDebug>
#include <QAudioInput>
#include <QAudioOutput>

#include "volumebar.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    QAudioDeviceInfo getInputInfo() { return inputDeviceInfo; }
    QAudioDeviceInfo getOutputInfo() { return outputDeviceInfo; }

public slots:
    void inputDeviceChanged();
    void outputDeviceChanged();
    void emitAcceptSignal() { emit deviceIsSelected(); }
    void drawMicVolume(qreal value);

signals:
    void deviceIsSelected();


private:
    Ui::Settings *ui;

    // Device properties
    QAudioDeviceInfo inputDeviceInfo;
    QAudioDeviceInfo outputDeviceInfo;


};

#endif // SETTINGS_H
