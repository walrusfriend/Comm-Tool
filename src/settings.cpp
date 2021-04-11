#include <QAudioDeviceInfo>

#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings),
    inputDeviceInfo(QAudioDeviceInfo::defaultInputDevice()),
    outputDeviceInfo(QAudioDeviceInfo::defaultOutputDevice())
{
    ui->setupUi(this);

    // set input devices
    ui->cmb_input->addItem("Default", QVariant(QString()));
    for (auto &device: QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        if (device != QAudioDeviceInfo::defaultInputDevice())
            ui->cmb_input->addItem(device.deviceName(), QVariant::fromValue(device));
    }
    connect(ui->cmb_input, QOverload<int>::of(&QComboBox::activated),
            this, &Settings::slotInputDeviceChanged);

    // set output devices
    ui->cmb_output->addItem("Default", QVariant(QString()));
    for (auto &device: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)) {
        if (device != QAudioDeviceInfo::defaultOutputDevice())
            ui->cmb_output->addItem(device.deviceName(), QVariant::fromValue(device));
    }
    connect(ui->cmb_output, QOverload<int>::of(&QComboBox::activated),
            this, &Settings::slotOutputDeviceChanged);

    // Linking signals and slots
    connect(ui->m_mic_hsld, SIGNAL(valueChanged(int)),
            this, SIGNAL(signalMicVolumeChanged(int)));
    connect(ui->m_phones_hsld, SIGNAL(valueChanged(int)),
            this, SIGNAL(signalPhonesVolumeChanged(int)));

    // Send signal when chech box is checked
    connect(ui->chb_hearYourself, SIGNAL(stateChanged(int)),
            this, SIGNAL(signalHearYourselfStateChanged(int)));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::slotInputDeviceChanged()
{
    QString soughtName = ui->cmb_input->currentText();

    if (soughtName == "Default") {
        inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
        emit signalDeviceIsSelected({inputDeviceInfo, outputDeviceInfo});
        return;
    }

    for (auto &device: QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        if (device.deviceName() == soughtName) {
            inputDeviceInfo = device;
            emit signalDeviceIsSelected({inputDeviceInfo, outputDeviceInfo});
            return;
        }
    }
    qWarning("ERROR: Couldn't find this device!");
}

void Settings::slotOutputDeviceChanged()
{
    QString soughtName = ui->cmb_output->currentText();

    if (soughtName == "Default") {
        outputDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();
        emit signalDeviceIsSelected({inputDeviceInfo, outputDeviceInfo});
        return;
    }

    for (auto &device: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)) {
        if (device.deviceName() == soughtName) {
            outputDeviceInfo = device;
            emit signalDeviceIsSelected({inputDeviceInfo, outputDeviceInfo});
            return;
        }
    }
    qWarning("ERROR: Couldn't find this device!");
}

void Settings::drawMicVolume(qreal& value) { ui->vb_mic->setLevel(value); }

void Settings::drawPhonesVolume(qreal& value) { ui->vb_phones->setLevel(value); }

bool Settings::isHearYourself() { return ui->chb_hearYourself->isChecked(); }
