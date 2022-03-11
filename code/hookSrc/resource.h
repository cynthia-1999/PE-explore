#ifndef RESOURCE_H
#define RESOURCE_H

#include <QDialog>

class Resource:public QDialog
{
    Q_OBJECT
public:
    Resource(QWidget *parent=nullptr);
    ~Resource();

protected:
    void paintEvent(QPaintEvent *);

    void timerEvent(QTimerEvent *);

    virtual float usage() const;

private:
    QPixmap pixmap;
    int base;
    int columnInc; // 动态列偏移值

    int *pts; // 随机数据数组
    int mLen; // 记录数组长度
    int ptNum; // 点数目
};

#endif // RESOURCE_H
