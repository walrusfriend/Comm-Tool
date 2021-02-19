#ifndef MYIODEVICE_H
#define MYIODEVICE_H

#include <QtCore/QIODevice>
#include <iostream>
#include <QtMultimedia/QAudioFormat>

class MyIODevice : public QIODevice
{
    Q_OBJECT

public:
    MyIODevice(QAudioFormat &format);

protected:
    qint64 readData(char* data, qint64 maxSize) override;
    qint64 writeData(const char* data, qint64 maxSize) override;

private:
    QAudioFormat format;
};

#endif // MYIODEVICE_H
