#include "matdisplay.h"

matDisplay::matDisplay(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);

    // Configura o tamanho do QLabel e cria uma imagem preta
    QSize size(731, 511);
    QImage image(size, QImage::Format_RGB32);

    // Preenche a imagem com preto
    image.fill(Qt::black);

    // Define a imagem inicial no QLabel
    this->setPixmap(QPixmap::fromImage(image));
}

void matDisplay::mouseMoveEvent(QMouseEvent *mouse_event)
{
    QPoint mouse_pos = mouse_event->pos();

    if (mouse_pos.x() <= this->size().width() && mouse_pos.y() <= this->size().height()) {
        if (mouse_pos.x() > 0 && mouse_pos.y() > 0) {
            emit sendMousePosition(mouse_pos);
        }
    }
}

void matDisplay::drawPixel(const QPoint &point)
{
    // Obtém a imagem atual
    QImage image = this->pixmap().toImage();

    // Cria um QPainter para desenhar na imagem
    QPainter painter(&image);

    // Define a cor e espessura do pincel
    painter.setPen(QPen(QColor(0, 0, 255), 2));

    // Desenha um pixel na posição especificada
    painter.drawPoint(point);

    // Atualiza a imagem no widget
    this->setPixmap(QPixmap::fromImage(image));
}

void matDisplay::drawLine(const QPoint &p1, const QPoint &p2)
{
    // Obtém a imagem atual
    QImage image = this->pixmap().toImage();

    // Cria um QPainter para desenhar na imagem
    QPainter painter(&image);

    // Define a cor e espessura do pincel
    painter.setPen(QPen(QColor(0, 0, 255), 2));

    // Desenha uma linha entre os pontos especificados
    painter.drawLine(p1, p2);

    // Atualiza a imagem no widget
    this->setPixmap(QPixmap::fromImage(image));
}

void matDisplay::drawTriangle(const QPoint &center, int size, int angle1, int angle2)
{
    // Calculate the vertices of the triangle based on the center, size, and angles
    QPoint p1(center.x() + size * std::cos(qDegreesToRadians(angle1)),
              center.y() + size * std::sin(qDegreesToRadians(angle1)));
    QPoint p2(center.x() + size * std::cos(qDegreesToRadians(angle2)),
              center.y() + size * std::sin(qDegreesToRadians(angle2)));
    QPoint p3(center.x() + size * std::cos(qDegreesToRadians(angle2 + 120)),
              center.y() + size * std::sin(qDegreesToRadians(angle2 + 120)));

    // Draw the triangle
    drawLine(center, p1);
    drawLine(p1, p2);
    drawLine(p2, p3);
    drawLine(p3, center);
}

void matDisplay::mousePressEvent(QMouseEvent *mouse_event)
{
    QPoint mouse_pos = mouse_event->pos();

    if (mouse_pos.x() <= this->size().width() && mouse_pos.y() <= this->size().height())
    {
        if (mouse_pos.x() > 0 && mouse_pos.y() > 0)
        {
            if (mode == 1)
            {
                drawTriangle(mouse_pos, 50, 60, 0);
            }
        }
    }
//    QMessageBox msg;
//    if(mouse_event->button() == Qt::LeftButton) {
//        msg.setText("Botão esquerdo pressionado!");
//        msg.exec();
//    }

//    if (mouse_event->button() == Qt::RightButton) {
//        msg.setText("Botão direito pressionado!");
//        msg.exec();
//    }
}
