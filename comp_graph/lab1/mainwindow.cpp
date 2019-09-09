#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QtMath>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , a(0), angle(0), speed(0)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showWithDialog(){
    dialog = new Dialog(this);
    QScreen* screen = QApplication::screens().at(0);
    QSize size = screen->availableSize();
    int min = size.width() < size.height() ? size.width() : size.height();
    while((a * 2.732 + 20) < 100 || (a * 2.732 + 20) > min - 40){
        dialog->setModal(true);
        dialog->exec();
        a = dialog->getA();
        if ((a * 2.732 + 20) < 100 || (a * 2.732 + 20) > min - 40)
            QMessageBox::warning(this, "Неверное значение", "Введите значение в диапазоне: " + QString::number(ceil(80/2.732)) + " - " + QString::number(int((min - 60)/2.732)));
    }
    int fig_radius = int(a*1.366);
    setGeometry(size.width()/2 - fig_radius, size.height()/2 - fig_radius, fig_radius * 2 + 20, fig_radius * 2 + 20);
    show();

    tmr = new QTimer(this);
    tmr->setInterval(1000/60);
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateRotation()));
    tmr->start();
}

void MainWindow::updateRotation(){
    angle += speed;
    angle = fmod(angle, 360);
    repaint();
}

void MainWindow::paintEvent(QPaintEvent* event){
    if (width() > height())             // Если ширина окна больше высоты
        a = (height() - 20) / 2.732;    // Отступ от краёв - 10 пикселей
    else a = (width() - 20) / 2.732;

    QPainter painter(this); // Создаём объекты-живописцы
    painter.translate(width()/2, height()/2);

    double a_2 = a/2, a_5 = a/5, a_10 = a/10;
    double fig_distance = a*1.366;                      //расстояние от центра фигуры до её крайней точки
    double s_c_distance = a_2 + a * qSqrt(3)/6;         //растояние от центра фигуры до центра малой окружности

    QRectF big_rect = QRectF(-a_2, -a_2, a, a);             //большой квадрат
    QRectF smal_rect = QRectF(-a_5/2, -a_5/2, a_5, a_5);    //малый квадрат  

    painter.rotate(angle);
    painter.setPen(Qt::white);
    QLinearGradient gradient(-a_2, -a_2, a_2, a_2);
    gradient.setColorAt(0.3, Qt::yellow); gradient.setColorAt(0.6, Qt::magenta); gradient.setColorAt(0.8, Qt::red);
    painter.drawRect(big_rect);
    painter.fillRect(big_rect, gradient);

    int k_arr[] = {1, 1, -1, -1, 1, 1};                 //вспомогательный массив для построения треугольников
    int i = 0;
    QColor colors[8];
    colors[0] = QColor(0, 30, 255); colors[1] = QColor(50, 255, 0);
    colors[2] = QColor(0, 255, 255); colors[3] = QColor(255, 0, 255);
    colors[4] = QColor(255, 180, 0); colors[5] = QColor(0, 255, 135);
    colors[6] = QColor(255, 0, 140); colors[7] = QColor(0, 180, 255);
    for(double alpha = 0; alpha < M_PI * 2; alpha += M_PI_2){
        QPolygon polygon;
        painter.setBrush(colors[i]);
        polygon << QPoint(fig_distance * qCos(alpha), - fig_distance * qSin(alpha));
        polygon << QPoint(k_arr[i] * a_2, k_arr[i+1] * a_2);
        i++;
        polygon << QPoint(k_arr[i] * a_2, k_arr[i+1] * a_2);
        painter.drawPolygon(polygon);
    }
    for(double alpha = 0; alpha < M_PI * 2; alpha += M_PI_2){
        painter.setBrush(colors[i++]);
        int x_s_c = s_c_distance * qCos(alpha) - a_5/2;     //координата x квадрата, описанного вокруг малой окружности
        int y_s_c = s_c_distance * qSin(alpha) - a_5/2;     //координата y этого квадрата
        painter.drawEllipse(x_s_c, y_s_c, a_5, a_5);
    }

    painter.rotate(-2*angle - 45);
    QLinearGradient gradient1(a_2, -a_2, -a_2, a_2);
    gradient1.setColorAt(0.3, Qt::yellow); gradient1.setColorAt(0.6, Qt::magenta); gradient1.setColorAt(0.8, Qt::red);
    painter.setBrush(gradient1);
    painter.drawRect(smal_rect);
    for(double alpha = 0; alpha < M_PI * 2; alpha += M_PI_2){
        QPolygon polygon;
        polygon << QPoint((a_5 * fabs(1 - fabs(angle)/180) + a_10 * (1 + qSqrt(3))) * qCos(alpha), (a_5 * fabs(1 - fabs(angle)/180) + a_10 * (1 + qSqrt(3))) * qSin(alpha));
        polygon << QPoint(a_5 * qCos(alpha) * fabs(1 - fabs(angle)/180) + a_10 *(qCos(alpha) + qSin(alpha)), a_5 * qSin(alpha) * fabs(1 - fabs(angle)/180) + a_10 *(qSin(alpha) - qCos(alpha)));
        polygon << QPoint(a_5 * qCos(alpha) * fabs(1 - fabs(angle)/180) + a_10 *(qCos(alpha) - qSin(alpha)), a_5 * qSin(alpha) * fabs(1 - fabs(angle)/180) + a_10 *(qSin(alpha) + qCos(alpha)));
        painter.drawConvexPolygon(polygon);
    }
 }

// Обработчик события прокрутки колеса мыши
void MainWindow::wheelEvent(QWheelEvent* wheelevent)
{
    speed += double(wheelevent->delta()) / 1200;
    repaint(); // Обновляем окно
}
