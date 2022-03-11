#ifndef PE_UI_MODEL_H
#define PE_UI_MODEL_H
#include<QAbstractTableModel>
#include"headers.h"

class PE_UI_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PE_UI_Model(bool flag, quint64 imagebase,byte * buffer, quint32 size, QObject * parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QStringList header;

    void set_addressOp(int op);
private:
    void init();
    byte * inter_data;
    quint32 fsize;
    quint64 image_base;
    bool pe32Flag;
    int count = 0;
    //0 为peOffset
    //1 为rva
    //2 为va
    int address_op = 0;
};

#endif // PE_UI_MODEL_H
