#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QWheelEvent>
#include "dialog.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showWithDialog();

private:
    Ui::MainWindow *ui;
    Dialog *dialog;

    double angle, a, speed;

    QTimer *tmr;

    // Обработчик события перерисовки окна
    void paintEvent(QPaintEvent* event);

    // Обработчик события прокрутки колеса мыши
    void wheelEvent(QWheelEvent* );

private slots:
    void updateRotation();
};

#endif // MAINWINDOW_H
