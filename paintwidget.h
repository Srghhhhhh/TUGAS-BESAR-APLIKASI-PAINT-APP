#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPoint>

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = nullptr);

    void setColor(const QColor &color);
    void setBrushSize(int size);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QImage image;
    QPoint lastPoint;
    bool drawing;

    QColor currentColor;
    int brushSize;
};

#endif