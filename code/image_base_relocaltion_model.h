#ifndef IMAGE_BASE_RELOCALTION_MODEL_H
#define IMAGE_BASE_RELOCALTION_MODEL_H
#include<QAbstractTableModel>
#include"headers.h"
#include"pefile.h"
#include"util.h"
#include"parsepe.h"

class IMAGE_BASE_RELOCALTION_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    IMAGE_BASE_RELOCALTION_Model(peFile * pe, QObject * parent = Q_NULLPTR);


    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QStringList header;
    int row_count = 2;
    peFile * pefile;
    PIMAGE_BASE_RELOCATION pbr;

    QStringList type;
};

#endif // IMAGE_BASE_RELOCALTION_MODEL_H
