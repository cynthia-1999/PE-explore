#ifndef IMAGE_RESOURCE_DIR_MODEL_H
#define IMAGE_RESOURCE_DIR_MODEL_H
#include<QAbstractTableModel>
#include"headers.h"
#include"util.h"
#include"parsepe.h"
#include"pefile.h"

class IMAGE_RESOURCE_DIR_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    IMAGE_RESOURCE_DIR_Model(quint32 entryOffset, peFile * pe, QObject * parent);

    virtual int         columnCount(const QModelIndex &parent) const;
    virtual int         rowCount(const QModelIndex &parent) const;
    virtual QVariant    data(const QModelIndex &index, int role) const;
    virtual QVariant    headerData(int section, Qt::Orientation orientation, int role) const;
private:
    QStringList header;
    int row_count = 6;
    PIMAGE_RESOURCE_DIRECTORY prd = Q_NULLPTR;
    peFile * pefile;
};

#endif // IMAGE_RESOURCE_DIR_MODEL_H
