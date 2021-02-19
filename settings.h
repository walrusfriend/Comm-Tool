#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QDebug>
#include <QAudioInput>
#include <QAudioOutput>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    void blank() { qDebug() << "BLANK"; }

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
