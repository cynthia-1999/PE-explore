#ifndef IMAGE_DOS_SIGNATURE_H
#define IMAGE_DOS_SIGNATURE_H

#include<QAbstractTableModel>
class Image_Dos_Signature : public QAbstractTableModel
{
    Q_OBJECT
public:
    Image_Dos_Signature(quint32 addr,QObject * parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QStringList header;
    quint32 addr;
};

#endif // IMAGE_DOS_SIGNATURE_H
