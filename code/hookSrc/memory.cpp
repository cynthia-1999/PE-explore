#include "memory.h"

memory::memory(QWidget *parent) : Resource(parent)
{

}
float memory::usage() const
{
    // 用当前时间设置种子
    QTime time = QTime::currentTime();
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
}
