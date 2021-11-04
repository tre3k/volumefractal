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
 *  volumefractal is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with volumefractal.  If not, see <https://www.gnu.org/licenses/>.
 *
 *     Author: Kirill Pshenichnyi <pshcyrill@mail.ru>
 */


#include "mainwindow.h"
#include "qwidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
        this->setWindowTitle("FFT3D Average");
        this->setMinimumHeight(400);
        this->setMinimumWidth(800);

        BuildMenu();

        /* Create widgets */
        viewer = new Widgets::Viewer();
        viewer->setMinimumSize(10,10);
        amount = new Widgets::Amount();
        amount->setMinimumSize(10,10);


        QTabWidget *central_tab_widget = new QTabWidget();
        this->setCentralWidget(central_tab_widget);

        central_tab_widget->addTab(viewer, "Viewer");
        central_tab_widget->addTab(amount, "Amount");
}

void MainWindow::BuildMenu(){
        menu_bar.menu_bar = new QMenuBar();

        menu_bar.file_menu = new QMenu("&File");
        menu_bar.menu_bar->addMenu(menu_bar.file_menu);

        menu_bar.open = new QAction("&Open");
        menu_bar.file_menu->addAction(menu_bar.open);

        menu_bar.export_dat = new QAction("&Export intencity");
        menu_bar.file_menu->addAction(menu_bar.export_dat);


        menu_bar.exit= new QAction("&Exit");
        menu_bar.file_menu->addAction(menu_bar.exit);

        this->setMenuBar(menu_bar.menu_bar);
        connect(menu_bar.open, &QAction::triggered,
                this, &MainWindow::OpenFile);
        connect(menu_bar.export_dat, &QAction::triggered,
        this, &MainWindow::ExportIntencityDat);
        connect(menu_bar.exit, &QAction::triggered,
                this, &MainWindow::close);
}

void MainWindow::OpenFile(){
        filename = QFileDialog::getOpenFileName(nullptr,
                                                "Open FFT3D file",
                                                "",
                                                "*.raw");
        if(filename=="") return;

        FFT3D::Data data(0);
        data.ReadOnlyHeader(filename.toStdString());
        unsigned long size = data.size_x();

        viewer->setFileName(filename);
        viewer->SetMaxDepth(size);
        viewer->setCurrentDepth(size/2);

        amount->setFilename(filename);
        amount->setSize(size);
}

void MainWindow::ExportIntencityDat() {

}

/* VIEWER Widget */
Widgets::Viewer::Viewer(QWidget *parent) : QWidget(parent) {
        layout = new QHBoxLayout();
        layout_depth = new QVBoxLayout();
        plot_case_ampl = new iCasePlot2D();
        plot_case_phase = new iCasePlot2D();
        plot_case_ampl->plot2D->xAxis->setLabel("x, pix.");
        plot_case_ampl->plot2D->yAxis->setLabel("y, pix.");
        plot_case_phase->plot2D->xAxis->setLabel("x, pix.");
        plot_case_phase->plot2D->yAxis->setLabel("y, pix.");
        plot_case_phase->plot2D->ColorScale->axis()->setTicker(
		QSharedPointer<QCPAxisTickerPi>(new QCPAxisTickerPi)
		);
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

        connect(spin_box_depth, SIGNAL(valueChanged(int)),
                slider, SLOT(setValue(int)));
        connect(spin_box_depth, SIGNAL(valueChanged(int)),
		this, SLOT(ShowDepth(int)));
        connect(slider, SIGNAL(valueChanged(int)),
		spin_box_depth, SLOT(setValue(int)));

        _data = new FFT3D::Data2D(0);
}

void Widgets::Viewer::ShowDepth(int depth) {
        if(_filename == "") return;

        FFT3D::Data::Read2DLayerDepthFromFile(_filename.toStdString(),
					      _data, (unsigned long) depth);

        unsigned long size = _data->size();
        plot_case_ampl->plot2D->ColorMap->data()->setSize(size, size);
        plot_case_ampl->plot2D->ColorMap->data()->setRange(QCPRange(0, size),
							   QCPRange(0, size));

        for(unsigned long i=0; i < size; i++){
                for(unsigned long j = 0; j < size; j++){
                        plot_case_ampl->plot2D->ColorMap->data()->
				setCell(i,j,abs(_data->getValue(i,j)));
                        // plot_case_ampl->plot2D->ColorMap->data()->
			// 	setCell(i,j,_data->getValue(i,j).real());
                }
        }

        plot_case_ampl->plot2D->ColorMap->rescaleDataRange(true);
        plot_case_ampl->plot2D->rescaleAxes();
        plot_case_ampl->plot2D->replot();

        plot_case_phase->plot2D->ColorMap->data()->setSize(size,size);
        plot_case_phase->plot2D->ColorMap->data()->setRange(QCPRange(0, size),
							    QCPRange(0, size));

        for(unsigned long i=0; i<size; i++){
                for(unsigned long j=0; j<size; j++){
                        plot_case_phase->plot2D->ColorMap->data()->
				setCell(i, j, arg(_data->getValue(i, j)));
                }
        }

        plot_case_phase->plot2D->ColorMap->rescaleDataRange(true);
        plot_case_phase->plot2D->rescaleAxes();
        plot_case_phase->plot2D->replot();
}


/* Amount widget */

Widgets::Amount::Amount(QWidget *parent) : QWidget(parent) {
        /* init objects for place on widget */
        QVBoxLayout *layout = new QVBoxLayout();
        QHBoxLayout *plot_layout = new QHBoxLayout();
        QHBoxLayout *butt_layout = new QHBoxLayout();
        QPushButton *amount_button = new QPushButton("amount all depth");
        plot_case_ampl = new iCasePlot2D();
        plot_case_phase = new iCasePlot2D();

        /* set windgets and layouts schema */
        plot_layout->addWidget(plot_case_ampl);
        plot_layout->addWidget(plot_case_phase);
        butt_layout->addWidget(amount_button);
        butt_layout->addStretch();
        layout->addLayout(plot_layout);
        layout->addLayout(butt_layout);
        this->setLayout(layout);

        connect(amount_button, &QPushButton::released,
                this, &Amount::process);
}

void Widgets::Amount::setFilename(QString filename){
        _filename = filename;
}

void Widgets::Amount::setSize(int size){
        _size = size;
}

void Widgets::Amount::process(){
        if(_filename == "") return;
        if(_data != nullptr) delete _data;
        _data = new FFT3D::Data2D(_size);

        std::complex<DATA_TYPE> tmp;
        FFT3D::Data2D *tmp_data;
        tmp_data = new FFT3D::Data2D(_size);

        for(int depth = 0; depth < _size; depth++){
                FFT3D::Data::Read2DLayerDepthFromFile(_filename.toStdString(),
                                                      tmp_data, depth);

                for(int i=0; i<_size; i++){
                        for(int j=0; j<_size; j++){
                                tmp = _data->getValue(i, j);
                                tmp += tmp_data->getValue(i, j);
                                _data->setValue(i, j, tmp);
                        }
                }
        }

        delete tmp_data;

        /* ploting result */
        plot_case_ampl->plot2D->ColorMap->data()->setSize(_size, _size);
        plot_case_ampl->plot2D->ColorMap->data()->setRange(QCPRange(0, _size),
                                                           QCPRange(0, _size));

        for(unsigned long i=0; i < _size; i++){
                for(unsigned long j = 0; j < _size; j++){
                        plot_case_ampl->plot2D->ColorMap->data()->
                                setCell(i, j, abs(_data->getValue(i,j)));
                }
        }

        plot_case_ampl->plot2D->ColorMap->rescaleDataRange(true);
        plot_case_ampl->plot2D->rescaleAxes();
        plot_case_ampl->plot2D->replot();

        plot_case_phase->plot2D->ColorMap->data()->setSize(_size, _size);
        plot_case_phase->plot2D->ColorMap->data()->setRange(QCPRange(0, _size),
                                                            QCPRange(0, _size));

        for(unsigned long i=0; i<_size; i++){
                for(unsigned long j=0; j<_size; j++){
                        plot_case_phase->plot2D->ColorMap->data()->
                                setCell(i, j, arg(_data->getValue(i, j)));
                }
        }

        plot_case_phase->plot2D->ColorMap->rescaleDataRange(true);
        plot_case_phase->plot2D->rescaleAxes();
        plot_case_phase->plot2D->replot();
}
