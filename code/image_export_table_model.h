#ifndef IMAGE_EXPORT_TABLE_MODEL_H
#define IMAGE_EXPORT_TABLE_MODEL_H

#include<QAbstractTableModel>
#include"headers.h"
#include"pefile.h"
#include"util.h"
#include"parsepe.h"
class IMAGE_EXPORT_TABLE_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    IMAGE_EXPORT_TABLE_Model(peFile * pe ,QObject * parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    peFile *pefile;
    QStringList header;
    int row_count = 11;
    PIMAGE_EXPORT_DIRECTORY IEDArray = Q_NULLPTR;

};

#endif // IMAGE_EXPORT_TABLE_MODEL_H
