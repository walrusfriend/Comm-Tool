// Settings window
// There user can adjust the mic and phones volume

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
    qint8 getMicVolume() { return micVolume; }

public slots:
    void inputDeviceChanged();
    void outputDeviceChanged();
    void emitAcceptSignal() { emit deviceIsSelected(); }
    void drawMicVolume(qreal value);
    void micVolumeSliderValueChanged(int value);
    void phonesVolumeSliderValueChanged(int value);

signals:
    void deviceIsSelected();


private:
    Ui::Settings *ui;

    // Device properties
    QAudioDeviceInfo inputDeviceInfo;
    QAudioDeviceInfo outputDeviceInfo;
    qint8 micVolume;
    qint8 phonesVolume;


};

#endif // SETTINGS_H
