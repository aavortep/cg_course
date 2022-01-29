#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Metronome");

    ui->centralwidget->setStyleSheet("QWidget {background: rgba(50, 50, 50, 255);}");

    init_canvas();

    canvas->add_body(Point(0, 0, 0), QString("model/body.obj"), QColor(0, 0, 0));
    canvas->add_pendulum(Point(0, 0, 0), QString("model/pendulum.obj"), QColor(0, 0, 0));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_canvas()
{
    size_t w = ui->graphicsView->width();
    size_t h = ui->graphicsView->height();

    canvas = new Canvas(w, h, this);

    ui->graphicsView->setScene(canvas);
}

void MainWindow::on_inc_clicked()
{
    const double k = 1.2;
    canvas->scale_model(k, k, k);
    canvas->update();
    canvas->update_screen();
}

void MainWindow::on_dec_clicked()
{
    const double k = 0.8;
    canvas->scale_model(k, k, k);
    canvas->update();
    canvas->update_screen();
}

void MainWindow::on_move_clicked()
{
    const int dx = ui->dx->text().toInt();
    const int dy = ui->dy->text().toInt();
    const int dz = ui->dz->text().toInt();

    canvas->move_camera(dx, dy, dz);
    canvas->update();
    canvas->update_screen();
}

void MainWindow::on_rotate_clicked()
{
    const double ox = ui->ox->text().toDouble();
    const double oy = ui->oy->text().toDouble();
    const double oz = ui->oz->text().toDouble();

    canvas->rotate_camera(ox, oy, oz);
    canvas->update();
    canvas->update_screen();
}

void MainWindow::on_change_light_clicked()
{
    const int x = ui->x_light->text().toInt();
    const int y = ui->y_light->text().toInt();
    const int z = ui->z_light->text().toInt();
    const float power = ui->power->text().toFloat();
    const Point pos(x, y, z);

    canvas->set_light(pos, power);
    canvas->update();
    canvas->update_screen();
}
