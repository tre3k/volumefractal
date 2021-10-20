/*
 *  Copyright (c) 2020-2021 NRC KI PNPI, Gatchina, LO, 188300 Russia
 *
 *  This file is part of volumefractal (fft3daverage).
 *
 *  volumefractal is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 *
 *     Author: Kirill Pshenichnyi <pshcyrill@mail.ru>
 */


#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
        this->setWindowTitle("FFT3D Average");
        this->setMinimumHeight(400);
        this->setMinimumWidth(800);

        BuildMenu();

        viewer = new Widgets::Viewer();
        viewer->setMinimumSize(10,10);
        spherical_viewer = new Widgets::SphericalViewer();
        average = new Widgets::Average();


        QTabWidget *central_tab_widget = new QTabWidget();
        this->setCentralWidget(central_tab_widget);

        central_tab_widget->addTab(viewer,"Viewer");
        central_tab_widget->addTab(spherical_viewer,"Spherical Viewer");
        central_tab_widget->addTab(average,"Average");

        thread_average = new ThreadAverage();
        connect(average->button_average,SIGNAL(pressed()),thread_average,SLOT(start()));
        connect(thread_average,SIGNAL(finish(QVector<double>,QVector<double>)),average,SLOT(finish(QVector<double>,QVector<double>)));
        connect(thread_average,SIGNAL(progress(int)),average,SLOT(setProgress(int)));
        connect(thread_average,SIGNAL(remain(int)),average,SLOT(remainTime(int)));
}

/* VIEWER Widget */
Widgets::Viewer::Viewer(QWidget *parent) : QWidget(parent){
        layout = new QHBoxLayout();
        layout_depth = new QVBoxLayout();
        plot_case_ampl = new iCasePlot2D();
        plot_case_phase = new iCasePlot2D();
        plot_case_ampl->plot2D->xAxis->setLabel("x, pix.");
        plot_case_ampl->plot2D->yAxis->setLabel("y, pix.");
        plot_case_phase->plot2D->xAxis->setLabel("x, pix.");
        plot_case_phase->plot2D->yAxis->setLabel("y, pix.");
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
        plot_average = new iQCustomPlot();
        plot_average->xAxis->setLabel("r, arb. units");
        plot_average->yAxis->setLabel("Intencity");
        button_average = new QPushButton("average");
        progress_bar = new QProgressBar();
        remain_time = new QLabel("time left: ");

        auto leftLayout = new QVBoxLayout();
        auto layout = new QHBoxLayout();

        plot_average->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        progress_bar->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
        progress_bar->setMaximumHeight(10);
        progress_bar->setTextVisible(false);

        leftLayout->addWidget(button_average);
        leftLayout->addWidget(progress_bar);
        leftLayout->addWidget(remain_time);
        leftLayout->addStretch();

        layout->addWidget(plot_average);
        layout->addLayout(leftLayout);

        this->setLayout(layout);

}

void Widgets::Average::finish(QVector<double> r,QVector<double> intencity){
        plot_average->clearGraphs();
        plot_average->addCurve(&r,&intencity,true,QColor("black"),"test");
}


/* SphericalViewer Widget */
Widgets::SphericalViewer::SphericalViewer(QWidget *parent) : QWidget(parent){
        auto plotLayout = new QHBoxLayout();
        auto radiusLayout = new QVBoxLayout();
        auto layout = new QVBoxLayout();

        slider_radius = new QSlider();
        slider_radius->setRange(0,500);
        spin_box_radius = new QDoubleSpinBox();
        spin_box_radius->setRange(0,0.5); // * 1000
        spin_box_radius->setDecimals(5);
        spin_box_radius->setSingleStep(0.01);

        radiusLayout->addWidget(new QLabel("r, a.u."));
        radiusLayout->addWidget(spin_box_radius);
        radiusLayout->addWidget(slider_radius);

        plot_case_ampl = new iCasePlot2D();
        plot_case_phase = new iCasePlot2D();
        plot_case_ampl->plot2D->xAxis->setLabel("φ, deg.");
        plot_case_ampl->plot2D->yAxis->setLabel("θ, deg.");
        plot_case_phase->plot2D->xAxis->setLabel("φ, deg.");
        plot_case_phase->plot2D->yAxis->setLabel("θ, deg.");
        plot_case_phase->plot2D->ColorScale->axis()->setTicker(QSharedPointer<QCPAxisTickerPi>(new QCPAxisTickerPi));
        plot_case_ampl->slot_log(true);
        plot_case_ampl->checkBoxLog->setChecked(true);

        plotLayout->addWidget(plot_case_ampl);
        plotLayout->addWidget(plot_case_phase);
        plotLayout->addLayout(radiusLayout);

        layout->addLayout(plotLayout);

        this->setLayout(layout);

        connect(spin_box_radius,SIGNAL(valueChanged(double)),this,SLOT(Show(double)));
        connect(slider_radius,SIGNAL(valueChanged(int)),this,SLOT(syncSpinBox(int)));

        _data = new FFT3D::Data2D(0);
}


void Widgets::SphericalViewer::Show(double r){
        slider_radius->setValue(r*1000);

        if(_filename == "") return;
        //FFT3D::Data::Read2DLayerSphereFromFile(_filename.toStdString(), _data, phi, theta);
        //std::cout << FFT3D::Data::ReadValueSphere(_filename.toStdString(),0.1,phi,theta) << "\n";

        FFT3D::Data::Read2DLayerSphereFromFile(_filename.toStdString(), _data, r);

        unsigned long size = _data->size();
        plot_case_ampl->plot2D->ColorMap->data()->setSize(size,size);
        plot_case_ampl->plot2D->ColorMap->data()->setRange(QCPRange(-180,180),QCPRange(-180,180));

        for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                        plot_case_ampl->plot2D->ColorMap->data()->setCell(i,j,abs(_data->getValue(i,j)));
                }
        }

        plot_case_ampl->plot2D->ColorMap->rescaleDataRange(true);
        plot_case_ampl->plot2D->rescaleAxes();
        plot_case_ampl->plot2D->replot();

        plot_case_phase->plot2D->ColorMap->data()->setSize(size,size);
        plot_case_phase->plot2D->ColorMap->data()->setRange(QCPRange(-180,180),QCPRange(-180,180));

        for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                        plot_case_phase->plot2D->ColorMap->data()->setCell(i,j,arg(_data->getValue(i,j)));
                }
        }

        plot_case_phase->plot2D->ColorMap->rescaleDataRange(true);
        plot_case_phase->plot2D->rescaleAxes();
        plot_case_phase->plot2D->replot();
}
