#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QString>
#include <QWidget>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>

#include "fft3ddata.h"
#include "iqcustomplot.h"

namespace Widgets{

class Viewer : public QWidget{
    Q_OBJECT
private:
    QHBoxLayout *layout;
    QVBoxLayout *layout_depth;
    iCasePlot2D *plot_case_ampl;
    iCasePlot2D *plot_case_phase;
    QString _filename;
    FFT3D::Data2D *_data;
    QSlider *slider;
    QSpinBox *spin_box_depth;

public:
    Viewer(QWidget *parent = nullptr);

public slots:
    void ShowDepth(int depth);
    void setFileName(QString filename){_filename = filename;}

    void SetMaxDepth(unsigned long max_depth){
        slider->setRange(0,max_depth-1);
        spin_box_depth->setRange(0,max_depth-1);
    }
    void setCurrentDepth(int depth){
        slider->setValue(depth);
    }


};

}

/* MAIN WINDOW */
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

    Widgets::Viewer *viewer;

public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void OpenFile(void){
        filename = QFileDialog::getOpenFileName(nullptr,"Open FFT3D file","","*.dat");
        if(filename=="") return;

        FFT3D::Data data(0);
        data.ReadOnlyHeader(filename.toStdString());
        unsigned long size = data.size_x();

        viewer->setFileName(filename);
        viewer->SetMaxDepth(size);
        viewer->setCurrentDepth(size/2);
        //viewer->ShowDepth(size/2);
    }




signals:



};

#endif // MAINWINDOW_H
