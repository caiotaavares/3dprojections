#ifndef MATDISPLAY_H
#define MATDISPLAY_H

#include <QLabel>
#include <QObject>
#include <QWidget>

#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>

class matDisplay : public QLabel
{
    Q_OBJECT

public:
    matDisplay(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *mouse_event);
    void mousePressEvent(QMouseEvent *mouse_event);
    void drawPixel(const QPoint &point);
    void drawLine(const QPoint &pi, const QPoint &p2);

    void drawTriangle(const QPoint &center, int size, int angle1, int angle2);

    int mode = 0;

signals:
    void sendMousePosition(QPoint&);

};

#endif // MATDISPLAY_H
