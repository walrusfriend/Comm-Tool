#include "myiodevice.h"
#include <QDebug>
#include <qendian.h>

MyIODevice::MyIODevice(QAudioFormat &format)
    : format(format)
{

}

qint64 MyIODevice::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)
    return -1;
}

qint64 MyIODevice::writeData(const char *data, qint64 maxSize)
{
    const int channelBytes = format.sampleSize() / 8;
    const int sampleBytes = channelBytes * format.channelCount();
    const int numSamples = maxSize / sampleBytes;

    quint32 maxValue = 0;

    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(data);

    // s - sample, c - channel
    for (int s = 0; s < numSamples; ++s) {
        for (int c = 0; c < format.channelCount(); ++c) {
            quint32 value = 0;

            if (format.sampleSize() == 8 &&
                format.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
            }
            else if (format.sampleSize() == 8 &&
                     format.sampleType() == QAudioFormat::SignedInt) {
                    value = qAbs(*reinterpret_cast<const quint8*>(ptr));
            }
            else if (format.sampleSize() == 16 &&
                     format.sampleType() == QAudioFormat::UnSignedInt) {
                if  (format.byteOrder() == QAudioFormat::LittleEndian)
                    value = qFromLittleEndian<quint16>(ptr);
                else
                    value = qFromBigEndian<quint16>(ptr);
            }
            else if (format.sampleSize() == 16 &&
                     format.sampleType() == QAudioFormat::SignedInt) {
                if  (format.byteOrder() == QAudioFormat::LittleEndian)
                    value = qAbs(qFromLittleEndian<quint16>(ptr));
                else
                    value = qAbs(qFromBigEndian<quint16>(ptr));
            }
            else if (format.sampleSize() == 32 &&
                     format.sampleType() == QAudioFormat::SignedInt) {
                if  (format.byteOrder() == QAudioFormat::LittleEndian)
                    value = qFromLittleEndian<quint32>(ptr);
                else
                    value = qFromBigEndian<quint32>(ptr);
            }
            else if (format.sampleSize() == 32 &&
                     format.sampleType() == QAudioFormat::UnSignedInt){
                if  (format.byteOrder() == QAudioFormat::LittleEndian)
                    value = qAbs(qFromLittleEndian<quint32>(ptr));
                else
                    value = qAbs(qFromBigEndian<quint32>(ptr));
            }
            else if (format.sampleSize() == 32 &&
                     format.sampleType() == QAudioFormat::Float)
                value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff);
            qDebug() << value;
        }

        ptr += channelBytes;
    }

    return maxSize;
}
