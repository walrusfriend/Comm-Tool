#include <cmath>

#include "mainwindow.h"
#include "ui_mainwindow.h"

const qint16 BUFFER_SIZE = 14096;
const qint16 LIMIT_SAMPLE = 4096;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_buffer(BUFFER_SIZE, 0),
    m_maxAmplitude(0),
    m_settings(new Settings(this)),
    m_hostServer(new HostServer(this)),
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

    connect(this, SIGNAL(signalSendToPhones(qint16*, qint64&)),
            this, SLOT(slotWriteData(qint16*, qint64&)));

    // Change device after selection in combo box
    connect(m_settings, SIGNAL(signalDeviceIsSelected()), this, SLOT(slotChangeDevice()));

    // Parse address line and check for correct ip
    QRegExp exp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}:[0-9]{1,5}");
    QValidator* val = new QRegExpValidator(exp, this);
    ui->le_address->setValidator(val);

    // TCP client
    tcpClient = new TcpClient;
    connect(tcpClient, SIGNAL(signalSendTextToChat(QString)),
            ui->te_chat, SLOT(append(QString)));

    // connect TCP server to chat
    connect(m_hostServer, SIGNAL(signalSendTextToChat(QString)),
            ui->te_chat, SLOT(append(QString)));

    // When the enter key is pressed, emit a signal like a "connect" button
    connect(ui->le_address, SIGNAL(returnPressed()), ui->pbt_connect, SIGNAL(clicked()));

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

// Modifies the input data by changing microphone volume
qint16 MainWindow::applyMicVolumeToSample(qint16 iSample)
{
    return std::max(std::min((iSample * m_settings->getMicVolume() / 50), 32767), -32768);
}

qint16 MainWindow::applyPhonesVolumeToSample(qint16 iSample)
{
    return std::max(std::min((iSample * m_settings->getPhonesVolume() / 50), 32767), -32768);
}

void MainWindow::setNewDevice()
{
    createAudioInput();
    createAudioOutput();

    m_inputDevice.reset(m_audioInput->start());
    m_outputDevice = m_audioOutput->start();

    connect(m_inputDevice.data(), SIGNAL(readyRead()), this, SLOT(slotReadData()));
}

// Read data from input device
void MainWindow::slotReadData()
{
    // TO DO Set logarithmic scale for the volume
    // TO DO Fix stacked volume bars after close "Settings" window
    // check the number of samples in input buffer
    qint64 size = m_audioInput->bytesReady();

    // limit sample size
    if (size > LIMIT_SAMPLE)
        size = LIMIT_SAMPLE;

    // read the sound samples from input device to buffer
    qint64 len = m_inputDevice->read(m_buffer.data(), size);

    // if something read...
    if (len > 0) {
        // TO DO Depending on the format, defines the bit depth of the int (qint8, qint16...)
        qint16* outdata = reinterpret_cast<qint16*> (m_buffer.data());

        qint32 maxValue = 0;
        for (int i = 0; i < len; ++i) {
            // apply micphone volume to raw data
            outdata[i] = applyMicVolumeToSample(outdata[i]);

            // find max value for draw the volume bar
            qint32 value = outdata[i];
            maxValue = qMax(value, maxValue);
        }
        
        // find max value for estimate the volume of the microphone
        maxValue = qMin(maxValue, m_maxAmplitude);
        qreal newLevel = qreal(maxValue) / m_maxAmplitude;

        m_settings->drawMicVolume(newLevel);

        // write the sound sample to the output device to play back audio when check box is clicked
        if (m_settings->isHearYourself())
            emit signalSendToPhones(outdata, size);

        // TO DO Send sample to the udp socket instead of phones
    }
}

// Take data from the udp socket and send to the phones
void MainWindow::slotWriteData(qint16* outdata, qint64 &size)
{
    // Define max value to estimate and draw phones volume level
    qint32 maxValue = 0;
    for (int i = 0; i < size; ++i) {
        // Apply phones volume to data
        outdata[i] = applyPhonesVolumeToSample(outdata[i]);

        // find max value for draw the volume bar
        qint32 value = outdata[i];
        maxValue = qMax(value, maxValue);
    }
    maxValue = qMin(maxValue, m_maxAmplitude);
    qreal newLevel = qreal(maxValue) / m_maxAmplitude;
    m_settings->drawPhonesVolume(newLevel);

    m_outputDevice->write(reinterpret_cast<char*>(outdata), size);
}

void MainWindow::slotMenuAction(QAction* action)
{
    if (action->text() == "&Settings") {
        m_settings->show();
    }

    if (action->text() == "&Host server"){
        m_hostServer->show();
    }

    if (action->text() == "&Exit")
        qApp->exit();

    if (action->text() == "&About...")
        qDebug() << "We're in the \"about\" section!";
}

// Set new device
void MainWindow::slotChangeDevice()
{
    m_inputDeviceInfo = m_settings->getInputInfo();
    m_outputDeviceInfo = m_settings->getOutputInfo();

    setNewDevice();
}

void MainWindow::slotConnectToServer()
{
    QStringList dict = ui->le_address->text().split(":");
    tcpClient->connectToHost(dict[0], dict[1].toInt());
}

// if main widnow is closed, it closes all windows of the program
void MainWindow::closeEvent(QCloseEvent*)
{
    qApp->exit();
}
