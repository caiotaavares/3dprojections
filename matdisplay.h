#ifndef MATDISPLAY_H
#define MATDISPLAY_H

#include <QLabel>
#include <QObject>
#include <QWidget>

#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <stack>
#include <QPoint>

class matDisplay : public QLabel
{
    Q_OBJECT

public:
    matDisplay(QWidget *parent = 0);

    void setDrawMode(int newMode);

    void onFloodFillNeighborChanged(int index);

    enum class FloodFillNeighbor {
        Neighborhood4,
        Neighborhood8
    };

    FloodFillNeighbor currentFloodFillNeighbor = FloodFillNeighbor::Neighborhood4; // Default value is 4


protected:
    void mouseMoveEvent(QMouseEvent *mouse_event);
    void mousePressEvent(QMouseEvent *mouse_event);
    void drawPixel(const QPoint &point);
    void drawLine(const QPoint &pi, const QPoint &p2);

    void drawTriangle(const QPoint &center, int size, int angle1, int angle2);

    void floodFill(int x, int y, const QColor &newColor);

    int mode = 0;

signals:
    void sendMousePosition(QPoint&);

};

#endif // MATDISPLAY_H
