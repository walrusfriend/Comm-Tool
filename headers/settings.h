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
    bool isHearYourself();

public slots:
    void drawMicVolume(qreal& value);
    void drawPhonesVolume(qreal& value);
    void slotInputDeviceChanged();
    void slotOutputDeviceChanged();

signals:
    void signalHearYourselfStateChanged(int);
    void signalDeviceIsSelected(QList<QAudioDeviceInfo>);
    void signalMicVolumeChanged(int);
    void signalPhonesVolumeChanged(int);

private:
    Ui::Settings *ui;

    // Device properties
    QAudioDeviceInfo inputDeviceInfo;
    QAudioDeviceInfo outputDeviceInfo;
};

#endif // SETTINGS_H
