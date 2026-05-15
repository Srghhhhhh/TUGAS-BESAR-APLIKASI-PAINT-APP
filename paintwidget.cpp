#include "paintwidget.h"
#include <QPainter>
#include <QMouseEvent>

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget(parent), drawing(false), currentColor(Qt::black), brushSize(5)
{
    image = QImage(800, 600, QImage::Format_RGB32);
    image.fill(Qt::white);
}

void PaintWidget::setColor(const QColor &color) {
    currentColor = color;
}

void PaintWidget::setBrushSize(int size) {
    brushSize = size;
}

void PaintWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = true;
        lastPoint = event->pos();
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event) {
    if (drawing && (event->buttons() & Qt::LeftButton)) {
        QPainter painter(&image);
        painter.setPen(QPen(currentColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(lastPoint, event->pos());

        lastPoint = event->pos();
        update();
    }
}

void PaintWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}