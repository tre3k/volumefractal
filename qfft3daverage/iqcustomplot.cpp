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


#include "iqcustomplot.h"

iQCustomPlot::iQCustomPlot(QWidget *parent) : QCustomPlot(parent)
{
        this->setInteractions(QCP::iRangeZoom |
                              QCP::iRangeDrag |
                              QCP::iMultiSelect |
                              QCP::iSelectLegend |
                              QCP::iSelectPlottables |
                              QCP::iSelectAxes);

        this->xAxis2->setVisible(true);
        this->yAxis2->setVisible(true);

        connect(this->xAxis, SIGNAL(rangeChanged(QCPRange)),
		this->xAxis2, SLOT(setRange(QCPRange)));
        connect(this->yAxis, SIGNAL(rangeChanged(QCPRange)),
		this->yAxis2, SLOT(setRange(QCPRange)));

        connect(this,SIGNAL(axisClick(QCPAxis*,
				      QCPAxis::SelectablePart,
				      QMouseEvent*)),
                this,SLOT(slot_sAxies_drag_zoom(QCPAxis*,
						QCPAxis::SelectablePart,
						QMouseEvent*)));
        connect(this,SIGNAL(mouseDoubleClick(QMouseEvent*)),
                this,SLOT(slot_full_drag_zoom(QMouseEvent*)));
        connect(this,SIGNAL(selectionChangedByUser()),
                this,SLOT(slot_selectionChanged()));

        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this,SIGNAL(customContextMenuRequested(QPoint)),
                this,SLOT(slot_contextMenuReq(QPoint)));

        return;
}

void iQCustomPlot::slot_contextMenuReq(QPoint p){
	QMenu *menu = new QMenu(this);
	menu->setAttribute(Qt::WA_DeleteOnClose);
	QMenu *menu_export = new QMenu("Export");
	menu_export->setAttribute(Qt::WA_DeleteOnClose);
	menu->addMenu(menu_export);

	menu_export->addAction("bmp",this,SLOT(exportToBMP()));
	menu_export->addAction("jpg",this,SLOT(exportToJPG()));
	menu_export->addAction("pdf",this,SLOT(exportToPDF()));
	menu_export->addAction("png",this,SLOT(exportToPNG()));

	if(!x_log){
		menu->addAction("x log scale",this,SLOT(setXLog()));
	}else{
		menu->addAction("x linear scale",this,SLOT(setXLog()));
	}

	if(!y_log){
		menu->addAction("y log scale",this,SLOT(setYLog()));
	}else{
		menu->addAction("y linear scale",this,SLOT(setYLog()));
	}

	menu->popup(this->mapToGlobal(p));
}

void iQCustomPlot::exportToBMP(){
	auto filename = QFileDialog::getSaveFileName(nullptr,
						     "Save",
						     ".bmp",
						     "(*.bmp *.BMP)");
	if(filename!=nullptr) this->saveBmp(filename);
}

void iQCustomPlot::exportToJPG(){
	auto filename = QFileDialog::getSaveFileName(nullptr,
						     "Save",
						     ".jpg",
						     "(*.jpg *.JPG)");
	if(filename!=nullptr) this->saveJpg(filename);
}

void iQCustomPlot::exportToPDF(){
	auto filename = QFileDialog::getSaveFileName(nullptr,
						     "Save",
						     ".pdf",
						     "(*.pdf *.PDF)");
	if(filename!=nullptr) this->savePdf(filename);
}

void iQCustomPlot::exportToPNG(){
	auto filename = QFileDialog::getSaveFileName(nullptr,
						     "Save",
						     ".png",
						     "(*.png *.PNG)");
	if(filename!=nullptr) this->savePng(filename);
}


void iQCustomPlot::setYLog(){
	y_log = !y_log;
	if(y_log){
		this->yAxis->setScaleType(QCPAxis::stLogarithmic);
		this->yAxis2->setScaleType(QCPAxis::stLogarithmic);
	}else{
		this->yAxis->setScaleType(QCPAxis::stLinear);
		this->yAxis2->setScaleType(QCPAxis::stLinear);
	}
	this->replot();
}

void iQCustomPlot::setXLog(){
	x_log = !x_log;
	if(x_log){
		this->xAxis->setScaleType(QCPAxis::stLogarithmic);
		this->xAxis2->setScaleType(QCPAxis::stLogarithmic);
	}else{
		this->xAxis->setScaleType(QCPAxis::stLinear);
		this->xAxis2->setScaleType(QCPAxis::stLinear);
	}
	this->replot();
}



void iQCustomPlot::addCurve(QVector<double> *x, QVector<double> *y,
                            bool point_line, QColor color, QString name){
        int num = this->graphCount();
        this->addGraph();

        if(point_line){
                this->graph(num)->setScatterStyle(
			QCPScatterStyle(QCPScatterStyle::ssCircle,
					"gray",
					color, 5));
                // this->graph(num)->setSelectedPen(QPen(QColor(color),
		// 				      2,
		// 				      Qt::DotLine,
		// 				      Qt::SquareCap,
		// 				      Qt::BevelJoin));
                this->graph(num)->setPen(QPen(QColor(color),
					      1,
					      Qt::NoPen,
					      Qt::SquareCap,
					      Qt::BevelJoin));
        }else{
                // this->graph(num)->setSelectedPen(QPen(QColor(color),
		// 				      2,
		// 				      Qt::SolidLine,
		// 				      Qt::SquareCap,
		// 				      Qt::BevelJoin));
                this->graph(num)->setPen(QPen(QColor(color),
					      1,
					      Qt::SolidLine,
					      Qt::SquareCap,
					      Qt::BevelJoin));
        }

        this->graph(num)->setData(*x,*y);
        this->graph(num)->setName(name);

        this->graph(num)->selectionDecorator()->setPen(QPen(QColor(color),
							    2,
							    Qt::DotLine,
							    Qt::SquareCap,
							    Qt::BevelJoin));

        this->rescaleAxes(true);
        this->replot();
        return;
}


void iQCustomPlot::slot_sAxies_drag_zoom(QCPAxis* sAxis,
					 QCPAxis::SelectablePart part,
					 QMouseEvent* event) {
        this->axisRect()->setRangeDrag(sAxis->orientation());
        this->axisRect()->setRangeZoom(sAxis->orientation());
        return;
}

void iQCustomPlot::slot_full_drag_zoom(QMouseEvent *mouseEvent){
        this->axisRect()->setRangeDrag(this->xAxis->orientation()|
                                       this->yAxis->orientation());
        this->axisRect()->setRangeZoom(this->xAxis->orientation()|
                                       this->yAxis->orientation());
        return;
}

void iQCustomPlot::slot_selectionChanged(){
        if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
	    this->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            this->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) ||
	    this->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
	{
                this->xAxis2->setSelectedParts(QCPAxis::spAxis |
					       QCPAxis::spTickLabels);
                this->xAxis->setSelectedParts(QCPAxis::spAxis |
					      QCPAxis::spTickLabels);
        }

        // make left and right axes be selected synchronously,
	// and handle axis and tick labels as one selectable object:
        if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
	    this->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
	    this->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) ||
	    this->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
        {
                this->yAxis2->setSelectedParts(QCPAxis::spAxis|
					       QCPAxis::spTickLabels);
                this->yAxis->setSelectedParts(QCPAxis::spAxis|
					      QCPAxis::spTickLabels);
        }

        // synchronize selection of graphs with selection of corresponding
	// legend items:
        for (int i=0; i<this->graphCount(); ++i)
        {
                QCPGraph *graph = this->graph(i);
                QCPPlottableLegendItem *item = this->legend->
			itemWithPlottable(graph);
                if (item->selected() || graph->selected())
                {
                        item->setSelected(true);
                        graph->setSelection(QCPDataSelection(
						    graph->data()->dataRange()
						    ));
                }
        }
        return;
}

/* iQCustomPlot2D */
iQCustomPlot2D::iQCustomPlot2D(QWidget *parent) : iQCustomPlot(parent)
{
        ColorMap = new QCPColorMap(this->xAxis,this->yAxis);
        ColorScale = new QCPColorScale(this);
        ColorScale->setType(QCPAxis::atRight);
        this->plotLayout()->addElement(0,1,ColorScale);

        ColorMap->setColorScale(ColorScale);
        ColorMap->setGradient(QCPColorGradient::gpJet);

        return;
}

iCasePlot2D::iCasePlot2D(QWidget *parent) : QWidget(parent)
{
        vLayout = new QVBoxLayout;
        hLayout = new QHBoxLayout;
        checkBoxLog = new QCheckBox("log");
        checkBoxLog->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        //checkBoxLog->setLayoutDirection(Qt::RightToLeft);
        checkBoxManual = new QCheckBox("manual");
        checkBoxManual->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        rescaleButton = new QPushButton("rescale");

        plot2D = new iQCustomPlot2D;
        vLayout->addWidget(plot2D);
        vLayout->addLayout(hLayout);
        //hLayout->setDirection(QBoxLayout::Direction::RightToLeft);
        hLayout->addWidget(rescaleButton);
        hLayout->addSpacing(1000);
        //hLayout->addWidget(checkBoxManual);
        hLayout->addWidget(checkBoxLog);
        this->setLayout(vLayout);

        connect(checkBoxLog, SIGNAL(clicked(bool)),
		this, SLOT(slot_log(bool)));
        connect(checkBoxManual, SIGNAL(clicked(bool)),
		this, SLOT(slot_manual(bool)));
        connect(rescaleButton, SIGNAL(pressed()),
		this, SLOT(slot_rescale()));

        return;
}

void iCasePlot2D::slot_log(bool state){
        if(state){
                plot2D->ColorMap->setDataScaleType(QCPAxis::stLogarithmic);
        }else{
                plot2D->ColorMap->setDataScaleType(QCPAxis::stLinear);
        }
        plot2D->replot();
}

void iCasePlot2D::slot_manual(bool state){
        if(state){

        }else{

        }
        plot2D->replot();
}

void iCasePlot2D::slot_rescale(){
        this->plot2D->rescaleAxes();
        this->plot2D->ColorMap->rescaleDataRange(true);
        this->plot2D->replot();
}
