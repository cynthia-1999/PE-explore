#ifndef BINARY_UI_MODEL_H
#define BINARY_UI_MODEL_H

#include<QAbstractTableModel>
#include"headers.h"

class binary_ui_model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit binary_ui_model(byte * data, quint32 base, quint32 size, QObject * parent = Q_NULLPTR);


    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    byte * inter_data;
    quint32 base_address;
    quint32 data_size;
    QStringList header;
    int count = 0;
};

#endif // BINARY_UI_MODEL_H
