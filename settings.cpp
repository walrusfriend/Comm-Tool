#include <QAudioDeviceInfo>

#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings),
    inputDeviceInfo(QAudioDeviceInfo::defaultInputDevice()),
    outputDeviceInfo(QAudioDeviceInfo::defaultOutputDevice()),
    micVolume(50),
    phonesVolume(50)
{
    ui->setupUi(this);

    // set input devices
    ui->cmb_input->addItem("Default", QVariant(QString()));
    for (auto &device: QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        if (device != QAudioDeviceInfo::defaultInputDevice())
            ui->cmb_input->addItem(device.deviceName(), QVariant::fromValue(device));
    }
    connect(ui->cmb_input, QOverload<int>::of(&QComboBox::activated), this, &Settings::inputDeviceChanged);

    // set output devices
    ui->cmb_output->addItem("Default", QVariant(QString()));
    for (auto &device: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)) {
        if (device != QAudioDeviceInfo::defaultOutputDevice())
            ui->cmb_output->addItem(device.deviceName(), QVariant::fromValue(device));
    }
    connect(ui->cmb_output, QOverload<int>::of(&QComboBox::activated), this, &Settings::outputDeviceChanged);

    // Sliders settings
    ui->m_mic_hsld->setValue(50);
    ui->m_phones_hsld->setValue(50);

    connect(ui->m_mic_hsld, SIGNAL(valueChanged(int)),
            parent, SLOT(micSliderValueChanged(int)));
    connect(ui->m_phones_hsld, SIGNAL(valueChanged(int)),
            parent, SLOT(phonesSliderValueChanged(int)));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::inputDeviceChanged()
{
    QString soughtName = ui->cmb_input->currentText();

    if (soughtName == "Default") {
        inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
        emit deviceIsSelected();
        return;
    }

    for (auto &device: QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        if (device.deviceName() == soughtName) {
            inputDeviceInfo = device;
            emit deviceIsSelected();
            return;
        }
    }
    qWarning("ERROR: Couldn't find this device!");
}

void Settings::outputDeviceChanged()
{
    QString soughtName = ui->cmb_output->currentText();

    if (soughtName == "Default") {
        outputDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();
        emit deviceIsSelected();
        return;
    }

    for (auto &device: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)) {
        if (device.deviceName() == soughtName) {
            outputDeviceInfo = device;
            emit deviceIsSelected();
            return;
        }
    }
    qWarning("ERROR: Couldn't find this device!");
}

void Settings::drawMicVolume(qreal value)
{
    ui->vb_mic->setLevel(value);
}

void Settings::micVolumeSliderValueChanged(int value)
{
    micVolume = value;
}

void Settings::phonesVolumeSliderValueChanged(int value)
{
    phonesVolume = value;
}
