#include "piechartwidget.h"

#include <QPainter>
#include <QtMath>

PieChartWidget::PieChartWidget(QWidget *parent) : QWidget(parent)
{
//    QPalette palette;
//    palette.setBrush(QPalette::Background,Qt::gray);
//    this->setPalette(palette);
    setStyleSheet("#mainWindow{border-image:url(:/SuperMarketManageSystem/img/bgp.jpg);}");
    qRow = -1;qx=qy=-1;
    shape = PieChart;
    color1 = Qt::cyan;
    color2 = Qt::darkCyan;
    qLabelsY = "y",qLabelsX = "x";
    isPaint = true;
}

void PieChartWidget::setData(QVector<double> values, QVector<QColor> colors, QVector<QString> labels)
{
    qValues = values;
    qColors = colors;
    qLabels = labels;
    repaint();
    update();
}

void PieChartWidget::setSelectedRow(int row)
{
    qRow = row;
}

void PieChartWidget::setShape(PieChartWidget::Shape s)
{
    this->shape = s;
}

void PieChartWidget::setXY(int x, int y)
{
    qx = x;qy = y;
}

void PieChartWidget::setIsPaint(bool ok)
{
    isPaint = ok;
}

void PieChartWidget::setRefresh(PieChartWidget::Shape s)
{
    shape = s;
    update();
    repaint();
}

void PieChartWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::white);
    painter.drawRect(0,0,this->width(),this->height());
    if(isPaint){
        QRectF sz = QRectF(this->width()-100,this->height()-60,40,20);
        painter.setBrush(color1);
        painter.drawRect(sz);
        painter.drawText(sz,"柱状图");
        sz = QRectF(this->width()-50,this->height()-60,40,20);
        painter.setBrush(color2);
        painter.drawRect(sz);
        painter.drawText(sz,"饼状图");
    }

    QRectF size,size1,size2;
    if(this->height() > this->width()){
        size = QRectF(30,30,this->width() - 60, this->width() - 60);
        size1 = QRectF(24,24,this->width() - 48, this->width() - 48);
        size2 = QRectF(36,36,this->width() - 72, this->width() - 72);
    }
    else{
        size = QRectF(30,30,this->height() - 60, this->height() - 60);
        size1 = QRectF(24,24,this->height() - 48, this->height() - 48);
        size2 = QRectF(36,36,this->height() - 72, this->height() - 72);
    }
    painter.setBackground(QBrush(Qt::white));
    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);

    double sum = 0.0, startAngle = 0.0,max = 0.0;
    double percent,angle,endAngle;

    for(int i=0; i < qValues.size(); ++i){
        sum += qValues[i];
        if(/*i!=qValues.size()-1&&*/qValues[i]>max)max=qValues[i];
    }
    if(shape == PieChart){
        if(qValues.size()==0)return;
        for(int i=0; i < qValues.size(); ++i){
            percent = qValues[i] / sum;
            angle = percent * 360.0;
            endAngle = startAngle + angle;
            painter.setBrush(qColors[i]);
            if(i==qRow){
                painter.drawPie(size1,startAngle*16,angle*16);
                if(qx!=-1&&qy!=-1&&qRow!=-1){
                    painter.drawText(QRectF(qx,qy,60,12),qLabels[qRow]+":");
                    painter.drawText(QRectF(qx+66,qy,60,12),QString("%1%").arg(percent*100.0));
                }
            }
            else
                painter.drawPie(size,startAngle*16,angle*16);
            startAngle = endAngle;
        }

    }else if(shape == BarChart){
        painter.setBrush(Qt::black);
        int w = this->width(),h = this->height(),s=10;
        painter.drawText(QRectF(0,10,12,30),"数量");
        painter.drawText(QRectF(w-110,h-20,30,12),"商品");
        painter.setPen(QPen(Qt::black,2));
        painter.drawLine(QLine(10,10,10,h-20));
        painter.drawLine(QLine(10,h-20,w-102,h-20));

        painter.setPen(pen);
        if(qValues.size()==0)return;
        int cap = (w-106)/ (qValues.size());//间隔
        if(cap>50)cap = 50;
        if(cap<=10)cap=11;
        max = max + max * 0.05; //放大max 5%
        for(int i=0;i<qValues.size();++i){
            percent = qValues[i]/max;
            int tH = (int)(percent * (h-20));
            painter.setBrush(qColors[i]);
            painter.drawRect(QRectF(s+10,h-tH-20,cap-10,tH));
            painter.drawText(QRectF(s+6,h-tH-40,cap,10),QString::number(qValues[i]));
            painter.drawText(QRectF(s+4,h-12,cap,30),qLabels[i]);
            s+=cap;
        }
    }else{
        painter.setBrush(Qt::black);
        int w = this->width(),h = this->height();
        double maxx = 0.0;
        painter.drawText(QRectF(0,10,12,30),qLabelsY);
        painter.drawText(QRectF(w-110,h-20,30,12),qLabelsX);
        painter.setPen(QPen(Qt::black,2));
        painter.drawLine(QLine(10,10,10,h-20));
        painter.drawLine(QLine(10,h-20,w-102,h-20));
        if(qValues1.size()==0)return;
        for(int i=0;i<qValues1.size();++i){
            for(int j=0;j<qValues1[i].size();++i){
                maxx = qMax(maxx,qValues1[i][j]);
            }
        }
        maxx = maxx+maxx*0.05;
        int cap = (this->width() - 40)/qValues1.size();
        for(int i=0;i<qValues1.size();++i){
            QVector<QLine> lines;lines.clear();
            int s = 0;
            for(int j=0;j<qValues1[i].size()-1;++j){
                percent = qValues1[i][j]/maxx;
                int tH1 = (int)(percent * (h-20));
                double percent2 = qValues1[i][j+1]/maxx;
                int tH2 = (int)(percent2 * (h-20));
                lines.push_back(QLine(s+cap,h-tH1-20,s+cap+cap,h-tH2-20));
                s = s+cap+cap;
            }
            painter.setBrush(qColors[i]);
            painter.drawLines(lines);
        }
    }
    for(int i=0; i< qValues.size(); ++i){
        painter.setBrush(qColors[i]);
        painter.drawRect(QRectF(this->width()-150,10+20*i,15,15));
        painter.drawText(QRectF(this->width()-130,10+20*i,100,15),qLabels[i]);
    }
}

void PieChartWidget::mousePressEvent(QMouseEvent *e)
{
    int tx = e->x(),ty = e->y(),tw = this->width(),th = this->height();
    if(tx>=tw-100&&tx<=tw-60&&ty>=th-60&&ty<=th-40&&color1 == Qt::cyan){
        color1 = Qt::darkCyan;
        color2 = Qt::cyan;
        shape = BarChart;
        this->qRow = -1;qx=qy=-1;
    }
    if(tx>=tw-50&&tx<=tw-10&&ty>=th-60&&ty<=th-40&&color2 == Qt::cyan){
        color2 = Qt::darkCyan;
        color1 = Qt::cyan;
        shape = PieChart;
        this->qRow = -1;qx=qy=-1;
    }
    update();
    repaint();
}

void PieChartWidget::mouseMoveEvent(QMouseEvent *e)
{
    int x = e->x(),y = e->y(),cx ,cy,r;
    if(this->height()>this->width()){
        cx = cy = (this->width() - 60)/2 + 30 ;
    }
    else{
        cx = cy = (this->height() - 60)/2 + 30;
    }
    r = cx - 30;
    if((x-cx)*(x-cx)+(y-cy)*(y-cy)<=r*r){//在圆内
        int d = qAbs(y-cy);
        double dd = qSqrt((double)((x-cx)*(x-cx)+(y-cy)*(y-cy)));
        double angle1 = qRadiansToDegrees(qAsin((double)d/dd));
        double sum = 0.0, startAngle = 0.0,max = -10.0;
        double percent,angle,endAngle;
        if(x<cx&&y<cy){
            angle1 = 180.0 - angle1;
        }else if(x<cx&&y>=cy){
            angle1 = 180.0 + angle1;
        }else if(x>=cx&&y>=cy){
            angle1 = 360.0 - angle1;
        }
        for(int i=0; i < qValues.size(); ++i){
            sum += qValues[i];
            if(i!=qValues.size()-1&&qValues[i]>max)max=qValues[i];
        }

        for(int i=0; i < qValues.size(); ++i){
            percent = qValues[i] / sum;
            angle = percent * 360.0;
            endAngle = startAngle + angle;
            if(angle1>=startAngle&&angle1<=endAngle){
                this->qRow = i;
                qx=x,qy=y;
                qPercent = percent;
                break;
            }
            startAngle = endAngle;
        }
    }
    else{
        this->qRow = -1;
        qx=-1,qy=-1;
    }
    update();
    repaint();
}
