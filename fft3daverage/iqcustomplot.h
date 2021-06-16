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
