#include "matdisplay.h"

#include <QPoint>
#include <stack>
#include <QVector3D>
#include <QTimer>

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

void matDisplay::create3DObject()
{
    rotationTimer = new QTimer(this);
    connect(rotationTimer, &QTimer::timeout, this, &matDisplay::updateRotation);
    rotationTimer->start(20); // Tempo em milissegundos entre as atualizações de rotação

    setFixedSize(500, 500);

    // Inicialize um cubo 3D
    float size = 20.0f;
    this->vertices.clear();  // Limpe os vértices antes de adicionar novos
    this->vertices << QVector3D(-size, -size, -size)
                   << QVector3D(size, -size, -size)
                   << QVector3D(size, size, -size)
                   << QVector3D(-size, size, -size)
                   << QVector3D(-size, -size, size)
                   << QVector3D(size, -size, size)
                   << QVector3D(size, size, size)
                   << QVector3D(-size, size, size);

    // Inicialize o zBuffer
    zBuffer.fill(std::numeric_limits<float>::infinity(), width() * height());

    // Projeção 3D para 2D
    QVector<QPoint> projectedPoints;
    for (const auto& vertex : vertices) {
        // Aplicar rotação
        float rotatedX = vertex.x() * cos(qDegreesToRadians(rotationY)) - vertex.z() * sin(qDegreesToRadians(rotationY));
        float rotatedZ = vertex.x() * sin(qDegreesToRadians(rotationY)) + vertex.z() * cos(qDegreesToRadians(rotationY));

        // Aplicar projeção
        float projectedX = rotatedX * cos(qDegreesToRadians(this->rotationX)) + vertex.y() * sin(qDegreesToRadians(this->rotationX));
        float projectedY = vertex.y() * cos(qDegreesToRadians(this->rotationX)) - rotatedX * sin(qDegreesToRadians(this->rotationX));

        // Escala e translada para o centro da tela
        int screenX = static_cast<int>(projectedX * 5) + width() / 2;
        int screenY = static_cast<int>(projectedY * 5) + height() / 2;

        projectedPoints.push_back(QPoint(screenX, screenY));
    }

    // Desenha linhas entre os pontos projetados
    drawLine(projectedPoints[0], projectedPoints[1]);
    drawLine(projectedPoints[1], projectedPoints[2]);
    drawLine(projectedPoints[2], projectedPoints[3]);
    drawLine(projectedPoints[3], projectedPoints[0]);
    drawLine(projectedPoints[4], projectedPoints[5]);
    drawLine(projectedPoints[5], projectedPoints[6]);
    drawLine(projectedPoints[6], projectedPoints[7]);
    drawLine(projectedPoints[7], projectedPoints[4]);
    drawLine(projectedPoints[0], projectedPoints[4]);
    drawLine(projectedPoints[1], projectedPoints[5]);
    drawLine(projectedPoints[2], projectedPoints[6]);
    drawLine(projectedPoints[3], projectedPoints[7]);

    // Redesenha a cena após criar o objeto 3D
    repaint();
}

void matDisplay::updateRotation()
{
    // Atualiza os ângulos de rotação para fazer o objeto girar
    rotationX += 1.0f;
    rotationY += 1.0f;

    // Cria um novo pixmap para desenhar
    QPixmap newPixmap(size());

    // Inicializa um pintor com o novo pixmap
    QPainter painter(&newPixmap);
    painter.fillRect(rect(), Qt::black);

    // Chama a função create3DObject para redesenhar o objeto 3D
    create3DObject();

    // Define o novo pixmap no QLabel
    setPixmap(newPixmap);

    // Emite o sinal para notificar a mudança na rotação
    emit rotationChanged();
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
}
