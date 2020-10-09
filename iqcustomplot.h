#ifndef IQCUSTOMPLOT_H
#define IQCUSTOMPLOT_H

#include "qcustomplot.h"

class iQCustomPlot : public QCustomPlot
{
    Q_OBJECT
public:
    explicit iQCustomPlot(QWidget *parent = 0);
    void addCurve(QVector<double> *,QVector<double> *,bool,QColor,QString);

private slots:
    void slot_sAxies_drag_zoom(QCPAxis *,QCPAxis::SelectablePart,QMouseEvent *);
    void slot_full_drag_zoom(QMouseEvent *);
    void slot_selectionChanged();

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
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;

public slots:
    void slot_log(bool);
    void slot_manual(bool);
};

#endif // IQCUSTOMPLOT_H
