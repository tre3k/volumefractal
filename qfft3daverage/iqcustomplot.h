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


#ifndef IQCUSTOMPLOT_H
#define IQCUSTOMPLOT_H

#include "qcustomplot.h"

class iQCustomPlot : public QCustomPlot
{
        Q_OBJECT
public:
        explicit iQCustomPlot(QWidget *parent = 0);
        void addCurve(QVector<double> *,
		      QVector<double> *,
		      bool,
		      QColor,
		      QString);

private slots:
        void slot_sAxies_drag_zoom(QCPAxis *,
				   QCPAxis::SelectablePart,
				   QMouseEvent *);
        void slot_full_drag_zoom(QMouseEvent *);
        void slot_selectionChanged();

protected:
    bool x_log = false;
    bool y_log = false;

protected slots:
        void slot_contextMenuReq(QPoint p);

        void setXLog();
        void setYLog();

        void exportToPDF();
        void exportToBMP();
        void exportToJPG();
        void exportToPNG();

};

class iQCustomPlot2D : public iQCustomPlot
{
        Q_OBJECT
public:
        explicit iQCustomPlot2D(QWidget *parent = 0);
        QCPColorMap *ColorMap;
        QCPColorScale *ColorScale;

};

class iCasePlot2D : public QWidget
{
        Q_OBJECT
public:
        explicit iCasePlot2D(QWidget *parent = 0);

        iQCustomPlot2D *plot2D;
        QCheckBox *checkBoxLog;
        QCheckBox *checkBoxManual;
        QPushButton *rescaleButton;

        QVBoxLayout *vLayout;
        QHBoxLayout *hLayout;

public slots:
        void slot_log(bool);
        void slot_manual(bool);
        void slot_rescale();


};

#endif // IQCUSTOMPLOT_H
