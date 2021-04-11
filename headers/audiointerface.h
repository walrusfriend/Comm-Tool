#ifndef AUDIOINTERFACE_H
#define AUDIOINTERFACE_H

#include <QWidget>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>

class AudioInterface : public QWidget
{
    Q_OBJECT
public:
    explicit AudioInterface(QWidget *parent = nullptr);

private:
    const qint16 BUFFER_SIZE = 14096;
    const qint16 LIMIT_SAMPLE = 4096;

    QAudioDeviceInfo m_inputDeviceInfo;
    QAudioDeviceInfo m_outputDeviceInfo;
    QScopedPointer<QAudioInput> m_audioInput;
    QScopedPointer<QAudioOutput> m_audioOutput;
    QScopedPointer<QIODevice> m_inputDevice;
    QIODevice* m_outputDevice; // I don't know why scoped pointer doesn't work
    QByteArray m_buffer;
    QAudioFormat m_format;
    qint32 m_maxAmplitude;
    qint16 micVolume;
    qint16 phonesVolume;
    bool isHearYourself;

private:
    void createAudioInput();
    void createAudioOutput();

    // Modify the raw data according to the volume value
    qint16 applyMicVolumeToSample(qint16 iSample);
    qint16 applyPhonesVolumeToSample(qint16 iSample);

    void setNewDevice();

public slots:
    void slotChangeDevice(QList<QAudioDeviceInfo>);

private slots:
    // Now we read from mic and send to phones, but later...
    void slotReadData();                    // Read data from mic and send to UdpSocket
    void slotWriteData(qint16*, qint64&);   // Write data from UdpSocket and send to phones
    void slotMicVolumeChanged(int volume);
    void slotPhonesVolumeChanged(int volume);
    void slotHearYourselfStateChanged(int state);

signals:
    void signalSendToPhones(qint16*, qint64&);
    void drawMicVolumeBar(qreal& newLevel);
    void drawPhonesVolumeBar(qreal& newLevel);

};

#endif // AUDIOINTERFACE_H
