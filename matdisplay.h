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
#include <QPolygon>

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

    void create3DObject();

    float getRotationX() const { return rotationX; }
    float getRotationY() const { return rotationY; }

    void drawPolygon(const QPolygon &polygon);
    void drawFilledPolygon(const QVector<QPoint> &polygon);

    void rotateCube(float angleX, float angleY, float angleZ);

private:
    QVector<QVector3D> vertices;  // Vertices do objeto 3D
    QVector<float> zBuffer;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotationZ = 0.0f;

    QTimer* rotationTimer;

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
    void rotationChanged();

};

#endif // MATDISPLAY_H
