#include "mainwindow.h"
#include "ui_mainwindow.h"

const int BUFFER_SIZE = 14096;
const int LIMIT_SAMPLE = 4096;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_buffer(BUFFER_SIZE, 0),
    m_settings(new Settings),
    ui(new Ui::MainWindow)
{
    // setup audio format
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);

    m_inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
    m_outputDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    // check for supprt our format for current I/O devices
    if (!m_inputDeviceInfo.isFormatSupported(m_format)) {
        qWarning("This format for input device is not supported. Apply nearest format.");
        m_format = m_inputDeviceInfo.nearestFormat(m_format);
    }
    if (!m_outputDeviceInfo.isFormatSupported(m_format)) {
        qWarning("This format for output device is not supported. Apply nearest format.");
        m_format = m_outputDeviceInfo.nearestFormat(m_format);
    }

    setNewDevice();

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createAudioInput()
{
    if (m_inputDevice != 0) {
        disconnect(m_inputDevice.data(), 0, this, 0);
        m_inputDevice.reset();
    }

    m_audioInput.reset(new QAudioInput(m_inputDeviceInfo, m_format, this));
}

void MainWindow::createAudioOutput()
{
    m_audioOutput.reset(new QAudioOutput(m_outputDeviceInfo, m_format, this));
}

int MainWindow::ApplyVolumeToSample(qint16 iSample)
{
    return std::max(std::min((iSample * m_volume / 50), 35535), -35535);
}

void MainWindow::setNewDevice()
{
    if (!m_inputDevice.isNull())
        m_audioInput->stop();
    if (m_outputDevice == nullptr)
        m_audioOutput->stop();

    createAudioInput();
    createAudioOutput();

    m_inputDevice.reset(m_audioInput->start());
    m_outputDevice = m_audioOutput->start();

    connect(m_inputDevice.data(), SIGNAL(readyRead()), this, SLOT(readMore()));
}

void MainWindow::readMore()
{
    // check the number of samples in input buffer
    qint64 size = m_audioInput->bytesReady();

    // limit sample size
    if (size > LIMIT_SAMPLE)
        size = LIMIT_SAMPLE;

    // read the sound samples from input device to buffer
    qint64 len = m_inputDevice->read(m_buffer.data(), size);

    // if something read...
    if (len > 0) {
        qint16* outdata = reinterpret_cast<qint16*>(m_buffer.data());

        // apply volume to raw data
        for (int i = 0; i < len; ++i)
            outdata[i] = ApplyVolumeToSample(outdata[i]);

        // write the sound sample to the output device to play back audio
        m_outputDevice->write(reinterpret_cast<char*>(outdata), size);
    }
}

void MainWindow::sliderValueChanged(int value)
{
    m_volume = value;
}

void MainWindow::menuAction(QAction* action)
{
    if (action->text() == "&Settings") {
        m_settings->show();

        // Change device after device is selected in combo box
        connect(m_settings, SIGNAL(deviceIsSelected()), this, SLOT(changeDevice()));
    }
    if (action->text() == "&About...")
        qDebug() << "We're in the \"about\" section!";
}

void MainWindow::changeDevice()
{
    m_inputDeviceInfo = m_settings->getInputInfo();
    m_outputDeviceInfo = m_settings->getOutputInfo();

    setNewDevice();
}
