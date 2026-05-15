#include <QtWidgets>
#include "scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent),
      modified(false),
      scribbling(false),
      myPenWidth(3),
      myPenColor(Qt::black),
      myTool(Pencil),
      myBrushStyle(Solid)
{
    setAttribute(Qt::WA_StaticContents);
    setMouseTracking(true);
}

bool ScribbleArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    previewImage = image;
    modified = false;
    update();
    return true;
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());
    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    }
    return false;
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::setTool(Tool tool)
{
    myTool = tool;
    // Update cursor
    switch (tool) {
    case Eraser:
        setCursor(Qt::CrossCursor);
        break;
    case FloodFill:
        setCursor(Qt::PointingHandCursor);
        break;
    default:
        setCursor(Qt::CrossCursor);
        break;
    }
}

void ScribbleArea::setBrushStyle(BrushStyle style)
{
    myBrushStyle = style;
}

void ScribbleArea::clearImage()
{
    pushUndoStack();
    image.fill(qRgb(255, 255, 255));
    previewImage = image;
    modified = true;
    update();
}

void ScribbleArea::undo()
{
    if (undoStack.isEmpty())
        return;
    redoStack.push(image);
    image = undoStack.pop();
    previewImage = image;
    modified = true;
    update();
}

void ScribbleArea::redo()
{
    if (redoStack.isEmpty())
        return;
    undoStack.push(image);
    image = redoStack.pop();
    previewImage = image;
    modified = true;
    update();
}

void ScribbleArea::pushUndoStack()
{
    if (undoStack.size() >= MAX_UNDO)
        undoStack.removeFirst();
    undoStack.push(image);
    redoStack.clear();
}

Qt::PenStyle ScribbleArea::qtPenStyle() const
{
    switch (myBrushStyle) {
    case Dashed: return Qt::DashLine;
    case Dotted: return Qt::DotLine;
    default:     return Qt::SolidLine;
    }
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    pressPoint = event->pos();
    lastPoint = event->pos();

    if (myTool == FloodFill) {
        pushUndoStack();
        floodFill(event->pos(), myPenColor);
        modified = true;
        update();
        return;
    }

    scribbling = true;
    pushUndoStack();
    previewImage = image;
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if (!scribbling)
        return;

    if (myTool == Pencil || myTool == Eraser) {
        drawLineTo(event->pos());
    } else {
        // Shape preview: redraw from saved previewImage
        QImage tempImage = previewImage;
        QPainter painter(&tempImage);
        QPen pen(myTool == Eraser ? Qt::white : myPenColor,
                 myPenWidth, qtPenStyle(),
                 Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.setRenderHint(QPainter::Antialiasing);

        QRect rect = QRect(pressPoint, event->pos()).normalized();
        switch (myTool) {
        case DrawRect:
            painter.drawRect(rect);
            break;
        case DrawEllipse:
            painter.drawEllipse(rect);
            break;
        case DrawLine:
            painter.drawLine(pressPoint, event->pos());
            break;
        default:
            break;
        }
        image = tempImage;
        update();
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !scribbling)
        return;

    if (myTool == Pencil || myTool == Eraser) {
        drawLineTo(event->pos());
    } else {
        // Commit shape to previewImage
        QPainter painter(&previewImage);
        QPen pen(myTool == Eraser ? Qt::white : myPenColor,
                 myPenWidth, qtPenStyle(),
                 Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.setRenderHint(QPainter::Antialiasing);

        QRect rect = QRect(pressPoint, event->pos()).normalized();
        switch (myTool) {
        case DrawRect:
            painter.drawRect(rect);
            break;
        case DrawEllipse:
            painter.drawEllipse(rect);
            break;
        case DrawLine:
            painter.drawLine(pressPoint, event->pos());
            break;
        default:
            break;
        }
        image = previewImage;
    }

    scribbling = false;
    modified = true;
    update();
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth  = qMax(width()  + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        previewImage = image;
        update();
    }
    QWidget::resizeEvent(event);
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    QColor color = (myTool == Eraser) ? Qt::white : myPenColor;
    int width    = (myTool == Eraser) ? myPenWidth * 3 : myPenWidth;

    painter.setPen(QPen(color, width, qtPenStyle(), Qt::RoundCap, Qt::RoundJoin));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawLine(lastPoint, endPoint);

    modified = true;
    int rad = (width / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void ScribbleArea::resizeImage(QImage *img, const QSize &newSize)
{
    if (img->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *img);
    *img = newImage;
}

void ScribbleArea::floodFill(const QPoint &pos, const QColor &fillColor)
{
    // Convert to Format_ARGB32 for pixel-level manipulation
    QImage &img = image;
    if (!img.rect().contains(pos))
        return;

    QRgb targetColor = img.pixel(pos);
    QRgb fill = fillColor.rgb();

    if (targetColor == fill)
        return;

    // BFS flood fill
    QQueue<QPoint> queue;
    queue.enqueue(pos);

    while (!queue.isEmpty()) {
        QPoint p = queue.dequeue();
        if (!img.rect().contains(p))
            continue;
        if (img.pixel(p) != targetColor)
            continue;

        // Scan left
        int x = p.x();
        while (x >= 0 && img.pixel(x, p.y()) == targetColor)
            x--;
        x++;

        // Scan right and fill
        bool spanAbove = false, spanBelow = false;
        while (x < img.width() && img.pixel(x, p.y()) == targetColor) {
            img.setPixel(x, p.y(), fill);

            if (!spanAbove && p.y() > 0 && img.pixel(x, p.y() - 1) == targetColor) {
                queue.enqueue(QPoint(x, p.y() - 1));
                spanAbove = true;
            } else if (spanAbove && p.y() > 0 && img.pixel(x, p.y() - 1) != targetColor) {
                spanAbove = false;
            }

            if (!spanBelow && p.y() < img.height() - 1 && img.pixel(x, p.y() + 1) == targetColor) {
                queue.enqueue(QPoint(x, p.y() + 1));
                spanBelow = true;
            } else if (spanBelow && p.y() < img.height() - 1 && img.pixel(x, p.y() + 1) != targetColor) {
                spanBelow = false;
            }
            x++;
        }
    }

    previewImage = image;
}
