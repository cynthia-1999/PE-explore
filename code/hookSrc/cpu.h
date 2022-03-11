#ifndef CPU_H
#define CPU_H
#include"resource.h"
#include <QtGui>

class cpu : public Resource
{
public:
    cpu(QWidget *parent);
    float usage() const;
};

#endif // CPU_H
