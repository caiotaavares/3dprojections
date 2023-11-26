#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->mat_Display, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(showMousePositions(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMousePositions(QPoint &pos)
{
    ui->labelMousePos->setText("x: " + QString::number(pos.x()) + ", y: " + QString::number(pos.y()));
}

