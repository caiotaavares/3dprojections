#include "matdisplay.h"

#include <QPoint>
#include <stack>

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

void matDisplay::floodFill(int x, int y, const QColor &newColor)
{
    // Obtém a imagem atual
    QImage image = this->pixmap().toImage();

    // Obtém a cor do pixel na posição (x, y)
    QColor originalColor = image.pixelColor(x, y);

    // Verifica se a cor original é igual à nova cor
    if (originalColor == newColor)
        return;

    // Cria uma pilha para armazenar as posições a serem verificadas
    std::stack<QPoint> stack;

    // Adiciona a posição inicial à pilha
    stack.push(QPoint(x, y));

    while (!stack.empty())
    {
        // Obtém a posição do topo da pilha
        QPoint current = stack.top();
        stack.pop();

        // Verifica se a posição está dentro dos limites da imagem
        if (current.x() >= 0 && current.x() < image.width() &&
            current.y() >= 0 && current.y() < image.height())
        {
            // Verifica se a cor do pixel é a cor original
            if (image.pixelColor(current) == originalColor)
            {
                // Pinta o pixel com a nova cor
                image.setPixelColor(current, newColor);

                // Adiciona os vizinhos à pilha para verificação
                if (currentFloodFillNeighbor == FloodFillNeighbor::Neighborhood4) {
                    stack.push(QPoint(current.x() + 1, current.y()));
                    stack.push(QPoint(current.x() - 1, current.y()));
                    stack.push(QPoint(current.x(), current.y() + 1));
                    stack.push(QPoint(current.x(), current.y() - 1));
                } else if (currentFloodFillNeighbor == FloodFillNeighbor::Neighborhood8) {
                    stack.push(QPoint(current.x() + 1, current.y()));
                    stack.push(QPoint(current.x() - 1, current.y()));
                    stack.push(QPoint(current.x(), current.y() + 1));
                    stack.push(QPoint(current.x(), current.y() - 1));
                    stack.push(QPoint(current.x() + 1, current.y() + 1));
                    stack.push(QPoint(current.x() - 1, current.y() - 1));
                    stack.push(QPoint(current.x() - 1, current.y() + 1));
                    stack.push(QPoint(current.x() + 1, current.y() - 1));
                }
            }
        }
    }

    // Atualiza a imagem no widget
    this->setPixmap(QPixmap::fromImage(image));
}

void matDisplay::onFloodFillNeighborChanged(int index)
{
    // index 0: Vizinhança 4, index 1: Vizinhança 8
    if (index == 0) {
        currentFloodFillNeighbor = FloodFillNeighbor::Neighborhood4;
    } else {
        currentFloodFillNeighbor = FloodFillNeighbor::Neighborhood8;
    }
}

void matDisplay::setDrawMode(int newMode)
{
    mode = newMode;
}

void matDisplay::mousePressEvent(QMouseEvent *mouse_event)
{
    QPoint mouse_pos = mouse_event->pos();

    if (mouse_pos.x() <= this->size().width() && mouse_pos.y() <= this->size().height()) {
        if (mouse_pos.x() > 0 && mouse_pos.y() > 0) {
            if (mode == 1) {
                drawTriangle(mouse_pos, 50, 60, 0);
            }

            if (mode == 2) {
                floodFill(mouse_pos.x(), mouse_pos.y(), QColor(255, 0, 0)); // Implementar a escolha de cores
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
