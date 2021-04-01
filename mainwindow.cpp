#include <qmath.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

const qint16 BUFFER_SIZE = 14096;
const qint16 LIMIT_SAMPLE = 4096;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_buffer(BUFFER_SIZE, 0),
    m_maxAmplitude(0),
    m_micVolume(0),
    m_phnVolume(50),
    m_settings(new Settings(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup audio format
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);

    m_maxAmplitude = pow(2, m_format.sampleSize()) / 2 - 1;

    // define default devices as I\O devices
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

    // Initialize I/O devices
    setNewDevice();

    // Change device after selection in combo box
    connect(m_settings, SIGNAL(deviceIsSelected()), this, SLOT(changeDevice()));

    // TCP server
    tcpServer = new TcpServer(port);
    connect(tcpServer, SIGNAL(sendTextToChat(QString)),
            ui->te_chat, SLOT(append(QString)));

    // TCP client
    tcpClient = new TcpClient;
    connect(tcpClient, SIGNAL(sendTextToChat(QString)),
            ui->te_chat, SLOT(append(QString)));

    // Layout
    ui->verticalLayout->addWidget(tcpClient);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Create audio input device with new audio device info
// The new data is presented as a class member (m_inputDeviceInfo)
void MainWindow::createAudioInput()
{
    if (m_inputDevice != 0) {
        disconnect(m_inputDevice.data(), 0, this, 0);
        m_inputDevice.reset();
    }

    m_audioInput.reset(new QAudioInput(m_inputDeviceInfo, m_format, this));
}

// Create audio output device with new audio device info
// The new data is presented as a class member (m_outputDeviceInfo)
void MainWindow::createAudioOutput()
{
    m_audioOutput.reset(new QAudioOutput(m_outputDeviceInfo, m_format, this));
}

// Modifies the input data by changing its volume
qint16 MainWindow::ApplyVolumeToSample(qint16 iSample)
{
    return std::max(std::min((iSample * m_micVolume / 50), 32767), -32768);
}

// Disables the old I/O devices and connect the new devices
// 1. Stops the existing devices
// 2. Create new I/O
// 3. Open I/O devices for read/write data
void MainWindow::setNewDevice()
{
    createAudioInput();
    createAudioOutput();

    m_inputDevice.reset(m_audioInput->start());
    m_outputDevice = m_audioOutput->start();

    connect(m_inputDevice.data(), SIGNAL(readyRead()), this, SLOT(readMore()));
}

// Read data from input device
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

        qint32 maxValue = 0;
        for (int i = 0; i < len; ++i) {
            // apply volume to raw data
            outdata[i] = ApplyVolumeToSample(outdata[i]);

            // find max value for draw the volume bar
            qint32 value = outdata[i];

            maxValue = qMax(value, maxValue);
        }
        
        // find max value for estimate the volume of the microphone
        maxValue = qMin(maxValue, m_maxAmplitude);
        qreal newLevel = qreal(maxValue) / m_maxAmplitude;

        m_settings->drawMicVolume(newLevel);

        // write the sound sample to the output device to play back audio
        m_outputDevice->write(reinterpret_cast<char*>(outdata), size);
    }
}

void MainWindow::micSliderValueChanged(int value)
{
    m_micVolume = value;
}

void MainWindow::phonesSliderValueChanged(int value)
{
    m_phnVolume = value;
}

void MainWindow::menuAction(QAction* action)
{
    if (action->text() == "&Settings") {
        m_settings->show();
    }

    if (action->text() == "&Exit")
        qApp->exit();

    if (action->text() == "&About...")
        qDebug() << "We're in the \"about\" section!";
}

// Set new device
void MainWindow::changeDevice()
{
    m_inputDeviceInfo = m_settings->getInputInfo();
    m_outputDeviceInfo = m_settings->getOutputInfo();

    setNewDevice();
}

void MainWindow::connectToServer()
{
    // TO DO Parse address line and check for correct ip
    QString host = ui->le_address->text();
    tcpClient->connectToHost(host, port);
}

// if main widnow is closed, it closes all windows of the program
void MainWindow::closeEvent(QCloseEvent*)
{
    qApp->exit();
}
