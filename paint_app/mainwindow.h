#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QToolBar>
#include <QActionGroup>
#include "scribblearea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void saveAsJpg();
    void penColor();
    void about();
    void onToolChanged(QAction *action);
    void onBrushStyleChanged(int index);
    void onBrushSizeChanged(int value);
    void undo();
    void redo();

private:
    void createActions();
    void createMenus();
    void createToolbar();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    ScribbleArea *scribbleArea;

    // Toolbar
    QToolBar    *toolBar;
    QActionGroup *toolGroup;
    QAction     *pencilAct;
    QAction     *eraserAct;
    QAction     *fillAct;
    QAction     *rectAct;
    QAction     *ellipseAct;
    QAction     *lineAct;
    QAction     *colorAct;
    QAction     *undoAct;
    QAction     *redoAct;
    QAction     *clearAct;
    QAction     *saveJpgAct;
    QAction     *openAct;

    // Brush controls
    QSlider     *sizeSlider;
    QLabel      *sizeLabel;
    QComboBox   *brushStyleCombo;

    // Color preview
    QLabel      *colorPreview;

    // Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QAction *aboutAct;
};

#endif // MAINWINDOW_H
