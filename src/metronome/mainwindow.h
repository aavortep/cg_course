#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canvas.h"

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
    Canvas *canvas;

    Point center_body, center_pend;
    Point light_pos;

    void init_canvas();

private slots:
    void on_inc_clicked();
    void on_dec_clicked();
    void on_move_clicked();
    void on_rotate_clicked();
    void on_change_light_clicked();
};
#endif // MAINWINDOW_H
