#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBoxFloodFillNeighbor->addItem("Vizinhança 4");
        ui->comboBoxFloodFillNeighbor->addItem("Vizinhança 8");

    connect(ui->mat_Display, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(showMousePositions(QPoint&)));
    connect(ui->comboBoxFloodFillNeighbor, QOverload<int>::of(&QComboBox::currentIndexChanged),
            ui->mat_Display, &matDisplay::onFloodFillNeighborChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMousePositions(QPoint &pos)
{
    ui->labelMousePos->setText("x: " + QString::number(pos.x()) + ", y: " + QString::number(pos.y()));
}


void MainWindow::on_pushButtonDrawTriangle_clicked()
{
    ui->mat_Display->setDrawMode(1);
}


void MainWindow::on_pushButtonFloodFill_clicked()
{
    ui->mat_Display->setDrawMode(2);
}

