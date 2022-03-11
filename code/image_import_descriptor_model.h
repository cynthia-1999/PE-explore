#ifndef IMAGE_IMPORT_DESCRIPTOR_MODEL_H
#define IMAGE_IMPORT_DESCRIPTOR_MODEL_H

#include<QAbstractTableModel>
#include"headers.h"
#include"util.h"
#include"parsepe.h"
#include"pefile.h"
class IMAGE_IMPORT_DESCRIPTION_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    IMAGE_IMPORT_DESCRIPTION_Model(peFile * pefile,QObject * parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    int row_count = 0;
    peFile * pefile;

    QStringList header;
    quint32 offset;
    static const char * charCO                    ;
    static const char * charTimeDateStamp         ;
    static const char * charFor                   ;
    static const char * charName                  ;
    static const char * charFirstThunk            ;
};

#endif // IMAGE_IMPORT_DESCRIPTOR_MODEL_H
