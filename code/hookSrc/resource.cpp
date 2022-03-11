#include "resource.h"
#include <QtGui>

Resource::Resource(QWidget *parent):QDialog(parent)
{
    this->resize(480,450);

    pts=NULL;

    base=10; // 边界基数为10
    columnInc=0;

    // 黑色背景图
    pixmap=QPixmap(width()-base*2,height()-base*2);
    pixmap.fill(Qt::black);

    // 定时器刷新线条
    startTimer(500);
}

Resource::~Resource()
{
    if(pts)
        delete pts;
}

void Resource::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    // 线条色
    QPen pen(QColor(46, 139, 87),1.0);
    painter.setPen(pen);

    // 在图像上作画
    painter.drawPixmap(10,10,pixmap);

    int graphW=pixmap.width();
    int graphH=pixmap.height();

    // 画15行直线
    int graphRow=graphH/15;
    for(int i=base;i<=graphH+base;i+=graphRow)
    {
        painter.drawLine(base,i,base+graphW,i);
    }

    // 动态列指示参数
    int graphColumn=graphW/15;
    if(columnInc==0)
    {
        columnInc=graphColumn;
    }

    // 画15列直线
    for(int j=base+columnInc;j<graphW+base;j+=graphColumn)
    {
        painter.drawLine(j,base,j,base+graphH);
    }

    --columnInc; // 改变x坐标值,使曲线产生运动效果

    if(pts==NULL)
    {
        pts=new int[graphW];
        mLen=graphW;
        ptNum=0;
    }else
    {
        painter.setPen(QPen(Qt::yellow,1.0));

        // 产生随机数模拟CPU使用情况
        pts[ptNum]=base+graphH*usage();

        // 遍历数组,从后面开始画曲线
        for(int j=base+graphW-ptNum,k=0;k<ptNum;k++,j++)
        {
            if(k!=0)
            {
                if(pts[k]!=pts[k-1])
                {
                    painter.drawLine(j-1,pts[k-1],j,pts[k]); // 画直线
                }else
                {
                    painter.fillRect(j,pts[k],1,1,Qt::SolidPattern); // 相等则画矩形
                }
            }
        }

        if(ptNum+2==mLen) // 曲线到达最左端
        {
            for(int i=1;i<ptNum;i++)
            {
                pts[i-1]=pts[i];  // 舍弃最前面2点,从后向前赋值
            }
            --ptNum;
        }else // 曲线在中间
        {
            ptNum++;
        }
    }
}


void Resource::timerEvent(QTimerEvent *e)
{
    // 定时刷新界面
    update();
}

float Resource::usage() const
{
#if 0
    // 用当前时间设置种子
    QTime time=QTime::currentTime();
    qint64 msecs=time.second()*1000+time.msec();
    qsrand(msecs);

    // 取(0-1)间小数值
    float usage=0.0;
    while(true)
    {
        usage=(qrand()%100)/100.0;
        if(usage>0.1&&usage<0.9)
            break;
    }
    return usage;
#endif
}
