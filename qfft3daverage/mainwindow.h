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
#include <QTabWidget>
#include <QDebug>
#include <QThread>
#include <QScreen>

#include "fft3ddata.h"
#include "iqcustomplot.h"

class ThreadAverage : public QThread{
        Q_OBJECT
public:
        ThreadAverage(){}
        QVector<double> radius;
        QVector<double> intencity;



        void run() override {
                if(_filename == "") return;

                radius.clear();
                intencity.clear();

                auto _data = new FFT3D::Data(0);
                _data->ReadOnlyHeader(_filename.toStdString());
                auto size = _data->size_x();
                auto dphi = 360.0/size;
                auto dtheta = 360.0/size;
                auto dr = 0.5/size;

                std::complex<DATA_TYPE> value {0,0};
                double dIntencity;
                int points = 0;

                QElapsedTimer timer;
                double time_elapsed = 0;
                double tremain;

                for(double r = 0; r < 0.5; r += dr){
                        timer.start();
                        points = 0;
                        dIntencity = 0;
                        for(double phi = 0; phi < 360.0; phi += dphi){
                                for(double theta = 0.0;
				    theta < 360.0;
				    theta += dtheta){
                                        value = FFT3D::Data::ReadValueSphere(
						_filename.toStdString(),
						r, phi, theta
						);
                                        dIntencity += abs(value)*abs(value);
                                        points ++;
                                }
                        }
                        intencity.append(dIntencity/points);
                        radius.append(r);
                        emit progress(1 + int(r*200.0));
                        time_elapsed += timer.elapsed();
                        tremain = 100.0*time_elapsed/(1+r*200.0) -
				time_elapsed;
                        // emit remain((timer.elapsed() *
			// 	     (100-int(r*200.0)))/1000);
                        emit remain((int)(tremain/1000));
                }

                emit finish(radius,intencity);
        }

private:
        QString _filename;

public slots:
        void setFileName(QString filename){_filename = filename;}

signals:
        void finish(QVector<double> ,QVector<double>);
        void progress(int);
        void remain(int);

};

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

	class SphericalViewer  : public QWidget{
		Q_OBJECT
	public:
		SphericalViewer(QWidget *parent = nullptr);

	private:
		iCasePlot2D *plot_case_ampl;
		iCasePlot2D *plot_case_phase;
		QSlider *slider_radius;
		QDoubleSpinBox *spin_box_radius;
		QString _filename;
		FFT3D::Data2D *_data;

	public slots:
		void Show(double r);
		void syncSpinBox(int val){
			spin_box_radius->setValue(val/1000.0);
		}
		void setFileName(QString filename){_filename = filename;}

	signals:

	};


	class Average : public QWidget {
		Q_OBJECT
	public:
		Average(QWidget *parent = nullptr);
		QPushButton *button_average;

	private:
		iQCustomPlot *plot_average;
		QProgressBar *progress_bar;
		QString _filename;
		QLabel *remain_time;

	public slots:
		void setFileName(QString filename){_filename = filename;}
		void finish(QVector<double> r,QVector<double> intencity);
		void setProgress(int progress){
			progress_bar->setValue(progress);
		}
		void remainTime(int val){
			remain_time->setText("left time: " +
					     QString::number(val)+" s.");
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
                QAction *exit;
        } menu_bar;

        void BuildMenu(void){
                menu_bar.menu_bar = new QMenuBar();

                menu_bar.file_menu = new QMenu("&File");
                menu_bar.menu_bar->addMenu(menu_bar.file_menu);

                menu_bar.open = new QAction("&Open");
                menu_bar.file_menu->addAction(menu_bar.open);


                menu_bar.exit= new QAction("&Exit");
                menu_bar.file_menu->addAction(menu_bar.exit);

                this->setMenuBar(menu_bar.menu_bar);
                connect(menu_bar.open, SIGNAL(triggered()),
			this, SLOT(OpenFile()));
                connect(menu_bar.exit, SIGNAL(triggered()),
			this, SLOT(close()));
        }

        /* filename */
        QString filename;

        Widgets::Viewer *viewer;
        Widgets::SphericalViewer *spherical_viewer;
        Widgets::Average *average;

        ThreadAverage *thread_average;

public:

        explicit MainWindow(QWidget *parent = nullptr);

public slots:
        void OpenFile(void){
                filename = QFileDialog::getOpenFileName(nullptr,
							"Open FFT3D file",
							"",
							"*.raw");
                if(filename=="") return;

                FFT3D::Data data(0);
                data.ReadOnlyHeader(filename.toStdString());
                unsigned long size = data.size_x();

                viewer->setFileName(filename);
                spherical_viewer->setFileName(filename);
                average->setFileName(filename);
                thread_average->setFileName(filename);
                viewer->SetMaxDepth(size);
                viewer->setCurrentDepth(size/2);
        }

signals:

};

#endif // MAINWINDOW_H
