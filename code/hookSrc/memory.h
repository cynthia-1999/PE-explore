#ifndef MEMORY_H
#define MEMORY_H

#include"resource.h"
#include <QtGui>

class memory: public Resource
{
public:
    memory(QWidget *parent);
    float usage() const;
};

#endif // MEMORY_H
