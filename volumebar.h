// Displays the audio volume using the colored bars

#ifndef VOLUMEBAR_H
#define VOLUMEBAR_H

#include <QWidget>
class VolumeBar : public QWidget
{
    Q_OBJECT

public:
    VolumeBar(QWidget* parent = nullptr);

    void setLevel(qreal value);
    qreal m_level = 0;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap m_pixmap;
};

#endif // VOLUMEBAR_H
