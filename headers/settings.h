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

public:
    quint8 getMicVolume() { return micVolume; }
    quint8 getPhonesVolume() { return phonesVolume; }
    void drawMicVolume(qreal value);
    void drawPhonesVolume(qreal value);
    bool isHearYourself();

public slots:
    void slotInputDeviceChanged();
    void slotOutputDeviceChanged();
    void slotMicVolumeSliderValueChanged(int value);
    void slotPhonesVolumeSliderValueChanged(int value);

signals:
    void signalDeviceIsSelected();


private:
    Ui::Settings *ui;

    // Device properties
    QAudioDeviceInfo inputDeviceInfo;
    QAudioDeviceInfo outputDeviceInfo;
    qint8 micVolume;
    qint8 phonesVolume;

};

#endif // SETTINGS_H
