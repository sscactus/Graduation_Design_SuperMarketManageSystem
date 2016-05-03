#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <QWidget>
#include "qchartviewer.h"

class ImagePanel : public QWidget
{
    Q_OBJECT
public:
    explicit ImagePanel(double data[], const char* labels[], QWidget *parent = 0);
    ~ImagePanel();

signals:

public slots:

private:
    QChartViewer *m_qViewer;
    XYChart *m_chart;
};

#endif // IMAGEPANEL_H
