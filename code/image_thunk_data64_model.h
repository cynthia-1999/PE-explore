#ifndef IMAGE_THUNK_DATA64_MODEL_H
#define IMAGE_THUNK_DATA64_MODEL_H
#include<QAbstractTableModel>
#include"headers.h"
#include"util.h"
#include"pefile.h"
class IMAGE_THUNK_DATA64_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    IMAGE_THUNK_DATA64_Model(peFile * pe, quint32 addr,byte * itdArray, QString dllName,QObject * parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    PIMAGE_THUNK_DATA64 itdArray = Q_NULLPTR;
    QString dllName ="";
    QStringList header;
    int row_count = 1;
    quint32 addr;
    peFile * pefile;
};

#endif // IMAGE_THUNK_DATA64_MODEL_H
