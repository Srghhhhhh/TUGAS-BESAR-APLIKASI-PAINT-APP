#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QSize>
#include <QStack>
#include <QPen>

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    // Tool enum
    enum Tool {
        Pencil,
        Eraser,
        FloodFill,
        DrawRect,
        DrawEllipse,
        DrawLine
    };

    // Brush style enum
    enum BrushStyle {
        Solid,
        Dashed,
        Dotted
    };

    explicit ScribbleArea(QWidget *parent = nullptr);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);

    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setTool(Tool tool);
    void setBrushStyle(BrushStyle style);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }
    Tool currentTool() const { return myTool; }

public slots:
    void clearImage();
    void undo();
    void redo();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void floodFill(const QPoint &pos, const QColor &fillColor);
    void pushUndoStack();
    Qt::PenStyle qtPenStyle() const;

    bool modified;
    bool scribbling;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QPoint lastPoint;
    QPoint pressPoint;  // for shape preview
    Tool myTool;
    BrushStyle myBrushStyle;

    QImage previewImage; // for shape preview overlay

    // Undo/Redo stacks
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;
    static const int MAX_UNDO = 20;
};

#endif // SCRIBBLEAREA_H
