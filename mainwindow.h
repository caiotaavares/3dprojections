#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void showMousePositions(QPoint &pos);

private slots:
    void on_pushButtonDrawTriangle_clicked();
    void on_pushButtonFloodFill_clicked();
};
#endif // MAINWINDOW_H
