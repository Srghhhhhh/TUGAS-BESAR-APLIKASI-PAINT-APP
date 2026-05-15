#include <QtWidgets>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "scribblearea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();
    createMenus();
    setWindowTitle(tr("Scribble"));
    resize(500, 500);
}
bool MainWindow::maybeSave()
{
    if (ui->scribbleArea->isModified()) {
        auto ret = QMessageBox::warning(this, tr("Scribble"),
                                        tr("The image has been modified.\nDo you want to save your changes?"),
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save)
            return saveFile("png");
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initialPath);

    if (fileName.isEmpty())
        return false;

    return ui->scribbleArea->saveImage(fileName, fileFormat.constData());
}
void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open"), this);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    penColorAct = new QAction(tr("&Pen Color"), this);
    connect(penColorAct, &QAction::triggered, this, &MainWindow::penColor);

    penWidthAct = new QAction(tr("&Pen Width"), this);
    connect(penWidthAct, &QAction::triggered, this, &MainWindow::penWidth);

    clearAct = new QAction(tr("&Clear"), this);
    connect(clearAct, &QAction::triggered,ui->scribbleArea, &ScribbleArea::clearImage);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
}
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);

    optionMenu = menuBar()->addMenu(tr("&Options"));
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addAction(clearAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}
void MainWindow:: closeEvent(QCloseEvent *event){
    if(maybeSave()){
        event->accept();
    }else{
        event->ignore();
    }
}
void MainWindow::open(){
    if(maybeSave()){
        QString fileName=QFileDialog::getOpenFileName(this,
                                                        tr("Open File"), QDir::currentPath());
        if(!fileName.isEmpty()){
            ui->scribbleArea->openImage(fileName);
        }
    }
}
void MainWindow::save(){
    QAction *action= qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}
void MainWindow::penColor(){
    QColor newColor=QColorDialog::getColor(ui->scribbleArea->penColor());
    if(newColor.isValid()){
       ui->scribbleArea->setPenColor(newColor);
    }
}
void MainWindow::penWidth(){
    bool ok;
    int newWidth=QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width: "),
                                        ui->scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if(ok){
        ui->scribbleArea->setPenWidth(newWidth);
    }
}
void MainWindow::about(){
    QMessageBox::about(this, tr("About Scribble"),
                       tr("<p>The <b>Scribble</b> example is awesome</p>"));
}

void MainWindow::on_btn_sketsa_clicked()
{
    ui->scribbleArea->setPenColor(Qt::black);
    ui->scribbleArea->setPenWidth(2);
}


void MainWindow::on_btn_erase_clicked()
{
    ui->scribbleArea->setPenColor(Qt::white);
    ui->scribbleArea->setPenWidth(20);
}


void MainWindow::on_btn_hitam_clicked()
{
    ui->scribbleArea->setPenColor(Qt::black);
}



void MainWindow::on_btn_merah_clicked()
{
    ui->scribbleArea->setPenColor(Qt::red);
}



void MainWindow::on_btn_coklat_clicked()
{
    ui->scribbleArea->setPenColor(QColor(173, 121, 70));
}



void MainWindow::on_btn_hijau_clicked()
{
    ui->scribbleArea->setPenColor(Qt::green);
}



void MainWindow::on_btn_biru_clicked()
{
    ui->scribbleArea->setPenColor(Qt::blue);
}



void MainWindow::on_btn_kuning_clicked()
{
    ui->scribbleArea->setPenColor(Qt::yellow);
}



void MainWindow::on_btn_pink_clicked()
{
    ui->scribbleArea->setPenColor(QColor(255, 85, 255));
}


void MainWindow::on_btn_skyblue_clicked()
{
    ui->scribbleArea->setPenColor(QColor(85, 255, 255));
}

void MainWindow::on_btn_ungumuda_clicked()
{
    ui->scribbleArea->setPenColor(QColor(134, 150, 255));
}



void MainWindow::on_btn_hijaumuda_clicked()
{
    ui->scribbleArea->setPenColor(QColor(0, 255, 127));
}



void MainWindow::on_btn_birumuda_clicked()
{
    ui->scribbleArea->setPenColor(QColor(58, 176,255));
}



void MainWindow::on_btn_ungutua_clicked()
{
    ui->scribbleArea->setPenColor(QColor(170, 0, 127));
}


void MainWindow::on_btn_ungutua_2_clicked()
{
    ui->scribbleArea->setPenColor(QColor(171, 171, 197));
}

void MainWindow::on_btn_hijaumudakali_clicked()
{
    ui->scribbleArea->setPenColor(QColor(176, 255, 183));
}


void MainWindow::on_btn_kuningmuda_clicked()
{
    ui->scribbleArea->setPenColor(QColor(244, 255, 120));
}
void MainWindow::on_btn_circle_clicked()
{
    ui->scribbleArea->setShape(ScribbleArea::Circle);

}
void MainWindow::on_btn_square_clicked()
{
    ui->scribbleArea->setShape(ScribbleArea::Square);
}
void MainWindow::on_btn_pen_clicked()
{
    ui->scribbleArea->setShape(ScribbleArea::Pen);
}