#ifndef CODE32_MODEL_H
#define CODE32_MODEL_H
#include<QAbstractTableModel>

#include"headers.h"
#include"util.h"

class Code32_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Code32_Model(QList<opClass32>*, QObject * parent = Q_NULLPTR);
    ~Code32_Model();

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<opClass32>* codeList = Q_NULLPTR;
    QStringList header;
};

#endif // CODE32_MODEL_H
