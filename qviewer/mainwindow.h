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

        class Amount : public QWidget{
        public:
                Amount(QWidget *parent = nullptr);
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
                QAction *export_dat;
                QAction *exit;
        } menu_bar;

        void BuildMenu(void);

        /* filename */
        QString filename;

        Widgets::Viewer *viewer;

public:

        explicit MainWindow(QWidget *parent = nullptr);

public slots:
        void OpenFile(void);
        void ExportIntencityDat(void);

signals:

};

#endif // MAINWINDOW_H
