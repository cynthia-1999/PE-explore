#ifndef CODE64_MODEL_H
#define CODE64_MODEL_H
#include"headers.h"
#include<QAbstractTableModel>
#include"util.h"

class Code64_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Code64_Model(QList<opClass64>* , QObject * parent = Q_NULLPTR);
    ~Code64_Model();

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    QList<opClass64>* codeList = Q_NULLPTR;
    QStringList header;
};

#endif // CODE64_MODEL_H
