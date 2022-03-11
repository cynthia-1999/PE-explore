#ifndef IMAGE_RESOURCE_DATA_ENTRY_MODEL_H
#define IMAGE_RESOURCE_DATA_ENTRY_MODEL_H
#include"QAbstractTableModel"
#include"pefile.h"
#include"util.h"

class IMAGE_RESOURCE_Data_Entry_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    IMAGE_RESOURCE_Data_Entry_Model(quint32 base_addr, peFile * pe,QObject * parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    quint32 base_addr = 0;
    peFile * pefile;

    QStringList header;
};

#endif // IMAGE_RESOURCE_DATA_ENTRY_MODEL_H
