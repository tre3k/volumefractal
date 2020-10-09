#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QString>


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    /* Main MenuBar */
    struct s_menu_bar{
        QMenuBar *menu_bar;
        QMenu *file_menu;
        QAction *open;
    } menu_bar;

    void BuildMenu(void){
        menu_bar.menu_bar = new QMenuBar();

        menu_bar.file_menu = new QMenu("&File");
        menu_bar.menu_bar->addMenu(menu_bar.file_menu);

        menu_bar.open = new QAction("&Open");
        menu_bar.file_menu->addAction(menu_bar.open);

        this->setMenuBar(menu_bar.menu_bar);
        connect(menu_bar.open,SIGNAL(triggered()),this,SLOT(OpenFile()));
    }

    /* filename */
    QString filename;

public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void OpenFile(void){
        filename = QFileDialog::getOpenFileName(nullptr,"Open FFT3D file","","*.dat");
    }



signals:



};

#endif // MAINWINDOW_H
