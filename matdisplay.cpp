#include "matdisplay.h"

#include <QPoint>
#include <stack>
#include <QVector3D>
#include <QTimer>

/**
 * CONSTRUTOR
 * @brief matDisplay::matDisplay
 * @param parent
 */
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
/*
 * AUXILIARES
 */
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

/*
 * FLOOD FILL
 */
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

/*
 * PROJEÇÕES 3D
 */
void matDisplay::create3DObject()
{
    // Configuração do timer para atualizar a rotação periodicamente
//    rotationTimer = new QTimer(this);
//    connect(rotationTimer, &QTimer::timeout, this, &matDisplay::rotateCube);
//    rotationTimer->start(16); // Tempo em milissegundos entre as atualizações de rotação

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

    // Lista de faces do cubo (índices dos vértices)
    QVector<QVector<int>> cubeFaces = {
        {0, 1, 2, 3},  // Face frontal
        {4, 5, 6, 7},  // Face traseira
        {0, 4, 5, 1},  // Lado esquerdo
        {2, 3, 7, 6},  // Lado direito
        {0, 3, 7, 4},  // Topo
        {1, 2, 6, 5}   // Base
    };

    // Vetor para armazenar os pontos projetados
    QVector<QVector<QPoint>> projectedPointsList;

    // Projeção 3D para 2D
    for (const auto &face : cubeFaces)
    {
        QVector<QPoint> projectedPoints;
        for (const auto &vertexIndex : face)
        {
            const QVector3D &vertex = vertices[vertexIndex];

            // Aplicar rotação em torno do eixo Y
            float rotatedX = vertex.x() * cos(qDegreesToRadians(rotationY)) - vertex.z() * sin(qDegreesToRadians(rotationY));
            float rotatedZ = vertex.x() * sin(qDegreesToRadians(rotationY)) + vertex.z() * cos(qDegreesToRadians(rotationY));

            // Aplicar rotação em torno do eixo X
            float rotatedY = vertex.y() * cos(qDegreesToRadians(rotationX)) - rotatedZ * sin(qDegreesToRadians(rotationX));
            float rotatedZ2 = vertex.y() * sin(qDegreesToRadians(rotationX)) + rotatedZ * cos(qDegreesToRadians(rotationX));

            // Aplicar rotação em torno do eixo Z
            float finalX = rotatedX * cos(qDegreesToRadians(rotationZ)) - rotatedY * sin(qDegreesToRadians(rotationZ));
            float finalY = rotatedX * sin(qDegreesToRadians(rotationZ)) + rotatedY * cos(qDegreesToRadians(rotationZ));

            // Projetar para o plano 2D
            float projectedX = finalX;
            float projectedY = finalY;

            // Escala e translada para o centro da tela
            int screenX = static_cast<int>(projectedX * 5) + width() / 2;
            int screenY = static_cast<int>(projectedY * 5) + height() / 2;

            // Adiciona o ponto projetado ao vetor
            projectedPoints.push_back(QPoint(screenX, screenY));
        }

        // Adiciona o vetor de pontos projetados ao vetor de listas de pontos
        projectedPointsList.push_back(projectedPoints);
    }

    // Desenha as linhas e polígonos preenchidos após o loop
    for (const auto &projectedPoints : projectedPointsList)
    {
        for (int i = 0; i < projectedPoints.size(); ++i)
        {
            int nextIndex = (i + 1) % projectedPoints.size();
            drawLine(projectedPoints[i], projectedPoints[nextIndex]);
        }

        drawPolygon(projectedPoints);
    }
}

void matDisplay::drawPolygon(const QPolygon &polygon)
{
    // Obtém a imagem atual
    QImage image = this->pixmap().toImage();

    // Cria um QPainter para desenhar na imagem
    QPainter painter(&image);

    // Define a cor do pincel para preencher o polígono
    painter.setBrush(QColor(0, 0, 255)); // Cor azul, você pode ajustar conforme necessário

    // Desenha o polígono preenchido
    painter.drawPolygon(polygon);

    // Atualiza a imagem no widget
    this->setPixmap(QPixmap::fromImage(image));
}

void matDisplay::drawFilledPolygon(const QVector<QPoint> &polygon)
{
    // Não aplicar transformações adicionais aqui, já que elas já foram aplicadas durante a projeção 3D

    // Obtém a imagem atual
    QImage image = this->pixmap().toImage();

    // Cria um QPainter para desenhar na imagem
    QPainter painter(&image);

    // Define a cor do pincel para preencher o polígono
    painter.setBrush(QColor(0, 0, 255)); // Cor azul, você pode ajustar conforme necessário

    // Desenha o polígono preenchido
    painter.drawPolygon(polygon);

    // Atualiza a imagem no widget
    this->setPixmap(QPixmap::fromImage(image));
}

void matDisplay::rotateCube(float angleX, float angleY, float angleZ)
{
    // Atualiza os ângulos de rotação para girar o cubo
    rotationX += angleX;
    rotationY += angleY;
    rotationZ += angleZ;

    // Cria um novo pixmap para desenhar
    QPixmap newPixmap(size());

    // Inicializa um pintor com o novo pixmap
    QPainter painter(&newPixmap);
    painter.fillRect(rect(), Qt::black);

    // Define o novo pixmap no QLabel
    setPixmap(newPixmap);

    // Redesenha o objeto 3D com as novas rotações
    create3DObject();

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
