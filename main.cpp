#include <QApplication>
#include <QWidget>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow audio;
    audio.show();

    return a.exec();
}
