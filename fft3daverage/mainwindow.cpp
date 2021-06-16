#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
        this->setWindowTitle("FFT3D Average");
        this->setMinimumHeight(400);
        this->setMinimumWidth(800);

        BuildMenu();

        viewer = new Widgets::Viewer();
        viewer->setMinimumSize(10,10);
        //this->setCentralWidget(viewer);

        average = new Widgets::Average();


        QTabWidget *central_tab_widget = new QTabWidget();
        this->setCentralWidget(central_tab_widget);

        central_tab_widget->addTab(viewer,"Viewer");
        central_tab_widget->addTab(average,"Average");
}

/* VIEWER Widget */

Widgets::Viewer::Viewer(QWidget *parent) : QWidget(parent){
        layout = new QHBoxLayout();
        layout_depth = new QVBoxLayout();
        plot_case_ampl = new iCasePlot2D();
        plot_case_phase = new iCasePlot2D();
        plot_case_phase->plot2D->ColorScale->axis()->setTicker(QSharedPointer<QCPAxisTickerPi>(new QCPAxisTickerPi));
        plot_case_ampl->slot_log(true);
        plot_case_ampl->checkBoxLog->setChecked(true);

        slider = new QSlider();
        spin_box_depth = new QSpinBox();


        layout_depth->addWidget(new QLabel("depth"));
        layout_depth->addWidget(spin_box_depth);
        layout_depth->addWidget(slider);

        layout->addWidget(plot_case_ampl);
        layout->addWidget(plot_case_phase);
        layout->addLayout(layout_depth);
        this->setLayout(layout);

        connect(spin_box_depth,SIGNAL(valueChanged(int)),slider,SLOT(setValue(int)));
        connect(spin_box_depth,SIGNAL(valueChanged(int)),this,SLOT(ShowDepth(int)));
        connect(slider,SIGNAL(valueChanged(int)),spin_box_depth,SLOT(setValue(int)));

        _data = new FFT3D::Data2D(0);
}


void Widgets::Viewer::ShowDepth(int depth){
        if(_filename=="") return;

        FFT3D::Data::Read2DLayerDepthFromFile(_filename.toStdString(),_data,(unsigned long) depth);

        unsigned long size = _data->size();
        plot_case_ampl->plot2D->ColorMap->data()->setSize(size,size);
        plot_case_ampl->plot2D->ColorMap->data()->setRange(QCPRange(0,size),QCPRange(0,size));

        for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                        plot_case_ampl->plot2D->ColorMap->data()->setCell(i,j,abs(_data->getValue(i,j)));
                        //plot_case_ampl->plot2D->ColorMap->data()->setCell(i,j,_data->getValue(i,j).real());
                }
        }

        /*
    plot_case->plot2D->ColorMap->data()->setRange(QCPRange(0,ui->SpinBoxSizeOfPixel->value()),
                                                   QCPRange(0,ui->SpinBoxSizeOfPixel->value()));
                                                   */
        plot_case_ampl->plot2D->ColorMap->rescaleDataRange(true);
        plot_case_ampl->plot2D->rescaleAxes();
        plot_case_ampl->plot2D->replot();

        plot_case_phase->plot2D->ColorMap->data()->setSize(size,size);
        plot_case_phase->plot2D->ColorMap->data()->setRange(QCPRange(0,size),QCPRange(0,size));

        for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                        plot_case_phase->plot2D->ColorMap->data()->setCell(i,j,arg(_data->getValue(i,j)));
                }
        }

        plot_case_phase->plot2D->ColorMap->rescaleDataRange(true);
        plot_case_phase->plot2D->rescaleAxes();
        plot_case_phase->plot2D->replot();
}

/* AVERAGE Widget */

Widgets::Average::Average(QWidget *parent) : QWidget(parent){

}