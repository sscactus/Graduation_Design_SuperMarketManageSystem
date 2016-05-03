#include "imagepanel.h"
#include <QSqlQuery>

ImagePanel::ImagePanel(double data[], const char *labels[], QWidget *parent) : QWidget(parent)
{
    m_qViewer = new QChartViewer(this);
    m_chart = new XYChart(400,300);
    m_chart->setPlotArea(35, 25, 335, 240);

    // set the color of bar.
    BarLayer* blayer = m_chart->addBarLayer(DoubleArray(data, (int)(sizeof(data)/sizeof(data[0]))), 0xFF);
    m_chart->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels)/sizeof(labels[0]))));

    blayer->setBarGap(0.5);

    m_qViewer->setChart(m_chart);
}

ImagePanel::~ImagePanel()
{
    if(m_qViewer!=NULL)delete m_qViewer;
    if(this->m_chart!=NULL)delete m_chart;
}
