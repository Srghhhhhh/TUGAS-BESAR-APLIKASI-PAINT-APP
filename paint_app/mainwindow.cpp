#include <QtWidgets>
#include "mainwindow.h"
#include "scribblearea.h"

MainWindow::MainWindow()
{
    scribbleArea = new ScribbleArea;
    setCentralWidget(scribbleArea);

    createActions();
    createMenus();
    createToolbar();

    setWindowTitle(tr("Paint App"));
    resize(1000, 700);
}

// ─────────────────────────────────────────────
//  UI Construction
// ─────────────────────────────────────────────
void MainWindow::createActions()
{
    openAct     = new QAction(QIcon::fromTheme("document-open",
                              style()->standardIcon(QStyle::SP_DirOpenIcon)),
                              tr("&Open"), this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveJpgAct  = new QAction(QIcon::fromTheme("document-save",
                              style()->standardIcon(QStyle::SP_DialogSaveButton)),
                              tr("&Save as JPG"), this);
    saveJpgAct->setShortcut(QKeySequence::Save);
    connect(saveJpgAct, &QAction::triggered, this, &MainWindow::saveAsJpg);

    undoAct = new QAction(QIcon::fromTheme("edit-undo",
                          style()->standardIcon(QStyle::SP_ArrowBack)),
                          tr("&Undo"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    redoAct = new QAction(QIcon::fromTheme("edit-redo",
                          style()->standardIcon(QStyle::SP_ArrowForward)),
                          tr("&Redo"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    clearAct = new QAction(QIcon::fromTheme("edit-clear",
                           style()->standardIcon(QStyle::SP_TrashIcon)),
                           tr("&Clear Canvas"), this);
    clearAct->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Delete);
    connect(clearAct, &QAction::triggered, scribbleArea, &ScribbleArea::clearImage);

    colorAct = new QAction(tr("🎨 Color"), this);
    connect(colorAct, &QAction::triggered, this, &MainWindow::penColor);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    // Tool actions (checkable)
    toolGroup = new QActionGroup(this);
    toolGroup->setExclusive(true);
    connect(toolGroup, &QActionGroup::triggered, this, &MainWindow::onToolChanged);

    pencilAct   = new QAction(tr("✏ Pencil"),   this);
    pencilAct->setCheckable(true);
    pencilAct->setChecked(true);
    pencilAct->setData(QVariant((int)ScribbleArea::Pencil));
    pencilAct->setShortcut(Qt::Key_P);
    toolGroup->addAction(pencilAct);

    eraserAct   = new QAction(tr("⬜ Eraser"),   this);
    eraserAct->setCheckable(true);
    eraserAct->setData(QVariant((int)ScribbleArea::Eraser));
    eraserAct->setShortcut(Qt::Key_E);
    toolGroup->addAction(eraserAct);

    fillAct     = new QAction(tr("🪣 Fill"),     this);
    fillAct->setCheckable(true);
    fillAct->setData(QVariant((int)ScribbleArea::FloodFill));
    fillAct->setShortcut(Qt::Key_F);
    toolGroup->addAction(fillAct);

    rectAct     = new QAction(tr("▭ Rectangle"), this);
    rectAct->setCheckable(true);
    rectAct->setData(QVariant((int)ScribbleArea::DrawRect));
    rectAct->setShortcut(Qt::Key_R);
    toolGroup->addAction(rectAct);

    ellipseAct  = new QAction(tr("⬭ Ellipse"),  this);
    ellipseAct->setCheckable(true);
    ellipseAct->setData(QVariant((int)ScribbleArea::DrawEllipse));
    ellipseAct->setShortcut(Qt::Key_O);
    toolGroup->addAction(ellipseAct);

    lineAct     = new QAction(tr("╱ Line"),      this);
    lineAct->setCheckable(true);
    lineAct->setData(QVariant((int)ScribbleArea::DrawLine));
    lineAct->setShortcut(Qt::Key_L);
    toolGroup->addAction(lineAct);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveJpgAct);
    fileMenu->addSeparator();
    fileMenu->addAction(clearAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolbar()
{
    toolBar = addToolBar(tr("Tools"));
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(24, 24));

    // File actions
    toolBar->addAction(openAct);
    toolBar->addAction(saveJpgAct);
    toolBar->addSeparator();

    // Undo/Redo
    toolBar->addAction(undoAct);
    toolBar->addAction(redoAct);
    toolBar->addAction(clearAct);
    toolBar->addSeparator();

    // Drawing tools
    toolBar->addAction(pencilAct);
    toolBar->addAction(eraserAct);
    toolBar->addAction(fillAct);
    toolBar->addAction(rectAct);
    toolBar->addAction(ellipseAct);
    toolBar->addAction(lineAct);
    toolBar->addSeparator();

    // Color button + preview
    toolBar->addAction(colorAct);

    colorPreview = new QLabel;
    colorPreview->setFixedSize(28, 28);
    colorPreview->setStyleSheet("background-color: black; border: 2px solid #888; border-radius: 4px;");
    colorPreview->setToolTip(tr("Current color"));
    toolBar->addWidget(colorPreview);
    toolBar->addSeparator();

    // Brush style
    toolBar->addWidget(new QLabel(tr(" Style: ")));
    brushStyleCombo = new QComboBox;
    brushStyleCombo->addItem(tr("Solid"),  QVariant(0));
    brushStyleCombo->addItem(tr("Dashed"), QVariant(1));
    brushStyleCombo->addItem(tr("Dotted"), QVariant(2));
    brushStyleCombo->setFixedWidth(80);
    connect(brushStyleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onBrushStyleChanged);
    toolBar->addWidget(brushStyleCombo);
    toolBar->addSeparator();

    // Size slider
    toolBar->addWidget(new QLabel(tr(" Size: ")));
    sizeSlider = new QSlider(Qt::Horizontal);
    sizeSlider->setRange(1, 50);
    sizeSlider->setValue(3);
    sizeSlider->setFixedWidth(120);
    sizeSlider->setToolTip(tr("Brush size"));
    connect(sizeSlider, &QSlider::valueChanged,
            this, &MainWindow::onBrushSizeChanged);
    toolBar->addWidget(sizeSlider);

    sizeLabel = new QLabel(tr("3"));
    sizeLabel->setFixedWidth(24);
    toolBar->addWidget(sizeLabel);
}

// ─────────────────────────────────────────────
//  Slots
// ─────────────────────────────────────────────
void MainWindow::onToolChanged(QAction *action)
{
    int toolVal = action->data().toInt();
    scribbleArea->setTool(static_cast<ScribbleArea::Tool>(toolVal));
}

void MainWindow::onBrushStyleChanged(int index)
{
    scribbleArea->setBrushStyle(static_cast<ScribbleArea::BrushStyle>(index));
}

void MainWindow::onBrushSizeChanged(int value)
{
    scribbleArea->setPenWidth(value);
    sizeLabel->setText(QString::number(value));
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor(), this,
                                             tr("Select Color"),
                                             QColorDialog::ShowAlphaChannel);
    if (newColor.isValid()) {
        scribbleArea->setPenColor(newColor);
        // Update preview swatch
        colorPreview->setStyleSheet(
            QString("background-color: %1; border: 2px solid #888; border-radius: 4px;")
                .arg(newColor.name()));
    }
}

void MainWindow::undo()
{
    scribbleArea->undo();
}

void MainWindow::redo()
{
    scribbleArea->redo();
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                               tr("Open Image"), QDir::currentPath(),
                               tr("Images (*.png *.jpg *.jpeg *.bmp)"));
        if (!fileName.isEmpty())
            scribbleArea->openImage(fileName);
    }
}

void MainWindow::saveAsJpg()
{
    QString initialPath = QDir::homePath() + "/untitled.jpg";
    QString fileName = QFileDialog::getSaveFileName(this,
                           tr("Save Image as JPG"),
                           initialPath,
                           tr("JPEG Image (*.jpg *.jpeg)"));
    if (fileName.isEmpty())
        return;

    // Ensure extension
    if (!fileName.endsWith(".jpg", Qt::CaseInsensitive) &&
        !fileName.endsWith(".jpeg", Qt::CaseInsensitive))
        fileName += ".jpg";

    if (scribbleArea->saveImage(fileName, "jpg"))
        QMessageBox::information(this, tr("Saved"),
                                 tr("Image saved to:\n%1").arg(fileName));
    else
        QMessageBox::warning(this, tr("Error"),
                             tr("Could not save the image."));
}

bool MainWindow::maybeSave()
{
    if (scribbleArea->isModified()) {
        auto ret = QMessageBox::warning(this, tr("Paint App"),
                       tr("The image has been modified.\nDo you want to save your changes?"),
                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile("jpg");
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::homePath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath);
    if (fileName.isEmpty())
        return false;
    return scribbleArea->saveImage(fileName, fileFormat.constData());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Paint App"),
        tr("<h3>Paint App</h3>"
           "<p>Fitur:</p>"
           "<ul>"
           "<li>Pensil, Penghapus, Paint Bucket</li>"
           "<li>Segi Empat, Lingkaran, Garis</li>"
           "<li>Gaya brush: Solid, Dashed, Dotted</li>"
           "<li>Pilih warna bebas (Color Dialog)</li>"
           "<li>Undo / Redo (Ctrl+Z / Ctrl+Y)</li>"
           "<li>Simpan ke JPG</li>"
           "</ul>"));
}
