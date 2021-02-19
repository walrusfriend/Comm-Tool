#include <QAudioDeviceInfo>

#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    qDebug() << "Create settings object";

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

    show();
}

Settings::~Settings()
{
    delete ui;
}
