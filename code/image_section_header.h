#ifndef IMAGE_NT_HEADER_MODEL_H
#define IMAGE_NT_HEADER_MODEL_H
#include<QAbstractTableModel>
#include"headers.h"
#include"util.h"
#include"parsepe.h"
class IMAGE_SECTION_HEADER_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    IMAGE_SECTION_HEADER_Model(quint32 addr, byte * data,QObject * parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool execute = false;
private:
    quint32 addr;
    byte * inter_data;
    QStringList header;

    int row_count = 10;
    QList<DWORD> charList;

    static const char *charName    ;
    static const char *charPV      ;
    static const char *charVA      ;
    static const char *charSOD     ;
    static const char *charPTRD    ;
    static const char *charPTR     ;
    static const char *charPTL     ;
    static const char *charNO      ;
    static const char *charNOL     ;
    static const char *charC       ;
};

#endif // IMAGE_NT_HEADER_MODEL_H
