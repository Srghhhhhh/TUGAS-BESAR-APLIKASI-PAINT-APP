#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class ScribbleArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();

    void on_btn_hitam_clicked();

    void on_btn_merah_clicked();

    void on_btn_coklat_clicked();

    void on_btn_hijau_clicked();

    void on_btn_biru_clicked();

    void on_btn_kuning_clicked();

    void on_btn_pink_clicked();

    void on_btn_skyblue_clicked();

    void on_btn_hijaumuda_clicked();

    void on_btn_ungutua_2_clicked();

    void on_btn_hijaumudakali_clicked();

    void on_btn_kuningmuda_clicked();
    void on_btn_circle_clicked();
    void on_btn_square_clicked();
    void on_btn_pen_clicked();
    void on_btn_sketsa_clicked();
    void on_btn_erase_clicked();
    void on_btn_ungumuda_clicked();
    void on_btn_birumuda_clicked();
    void on_btn_ungutua_clicked();
private:
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);
   // ScribbleArea *scribbleArea;
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QList<QAction *>saveAsActs;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *printAct;
    QAction *clearScreenAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *saveAct;
    QAction *clearAct;
    Ui::MainWindow *ui;
};

#endif
