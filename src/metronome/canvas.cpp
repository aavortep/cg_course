#include "canvas.h"

using namespace std;

Canvas::Canvas(QWidget *parent) : QLabel(parent)
{
    image = QImage(1571, 1030, QImage::Format_ARGB32);
    image.fill(QColor("white"));

    update();

    pen_color = QColor("black");
    pen.setColor(pen_color);

    brush_color = QColor(252, 233, 79);
}

void Canvas::update()
{
    this->setPixmap(QPixmap::fromImage(image));
}

void Canvas::clear()
{
    image.fill(QColor("white"));
    update();
}
