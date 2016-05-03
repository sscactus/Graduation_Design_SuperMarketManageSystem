#ifndef PIECHARTWIDGET_H
#define PIECHARTWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class PieChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PieChartWidget(QWidget *parent = 0);
    void setData(QVector<double> values, QVector<QColor> colors, QVector<QString> labels);
    void setSelectedRow(int row);
    enum Shape{PieChart,BarChart,LineChart};
    void setShape(Shape);
    void setXY(int x,int y);
    void setIsPaint(bool ok);
    void setRefresh(Shape s);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);


private:
    QVector<double> qValues;
    QVector<QColor> qColors;
    QVector<QString> qLabels;
    QVector<QVector<double> > qValues1;
    Shape shape;
    int qRow,qx,qy;
    double qPercent;
    QString qLabelsX,qLabelsY;
    QColor color1,color2;
    bool isPaint;
signals:

public slots:
};

#endif // PIECHARTWIDGET_H
