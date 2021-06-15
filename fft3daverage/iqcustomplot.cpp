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

    connect(this->xAxis, SIGNAL(rangeChanged(QCPRange)),this->xAxis2, SLOT(setRange(QCPRange)));
    connect(this->yAxis, SIGNAL(rangeChanged(QCPRange)), this->yAxis2, SLOT(setRange(QCPRange)));

    connect(this,SIGNAL(axisClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),
            this,SLOT(slot_sAxies_drag_zoom(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)));
    connect(this,SIGNAL(mouseDoubleClick(QMouseEvent*)),
            this,SLOT(slot_full_drag_zoom(QMouseEvent*)));
    connect(this,SIGNAL(selectionChangedByUser()),
            this,SLOT(slot_selectionChanged()));
    return;
}

void iQCustomPlot::addCurve(QVector<double> *x, QVector<double> *y,
                             bool point_line, QColor color, QString name){
    int num = this->graphCount();
    this->addGraph();

     if(point_line){
        this->graph(num)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,"gray",color,5));
        //this->graph(num)->setSelectedPen(QPen(QColor(color),2,Qt::DotLine,Qt::SquareCap,Qt::BevelJoin));
        this->graph(num)->setPen(QPen(QColor(color),1,Qt::NoPen,Qt::SquareCap,Qt::BevelJoin));
     }else{
        //this->graph(num)->setSelectedPen(QPen(QColor(color),2,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
        this->graph(num)->setPen(QPen(QColor(color),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
     }

    this->graph(num)->setData(*x,*y);
    this->graph(num)->setName(name);

    this->graph(num)->selectionDecorator()->setPen(QPen(QColor(color),2,Qt::DotLine,Qt::SquareCap,Qt::BevelJoin));

    this->rescaleAxes(true);
    this->replot();
    return;
}


void iQCustomPlot::slot_sAxies_drag_zoom(QCPAxis* sAxis,QCPAxis::SelectablePart part,QMouseEvent* event){
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
    if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
          this->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
      {
        this->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        this->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      }
      // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
      if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
          this->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
      {
        this->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        this->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      }

      // synchronize selection of graphs with selection of corresponding legend items:
      for (int i=0; i<this->graphCount(); ++i)
      {
        QCPGraph *graph = this->graph(i);
        QCPPlottableLegendItem *item = this->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected())
        {
          item->setSelected(true);
          graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
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

    plot2D = new iQCustomPlot2D;
    vLayout->addWidget(plot2D);
    vLayout->addLayout(hLayout);
    //hLayout->setDirection(QBoxLayout::Direction::RightToLeft);
    hLayout->addSpacing(1000);
    hLayout->addWidget(checkBoxManual);
    hLayout->addWidget(checkBoxLog);
    this->setLayout(vLayout);

    connect(checkBoxLog,SIGNAL(clicked(bool)),this,SLOT(slot_log(bool)));
    connect(checkBoxManual,SIGNAL(clicked(bool)),this,SLOT(slot_manual(bool)));

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
