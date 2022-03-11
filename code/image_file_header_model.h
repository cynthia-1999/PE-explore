#ifndef IMAGE_FILE_HEADER_MODEL_H
#define IMAGE_FILE_HEADER_MODEL_H
#include"headers.h"
#include<QAbstractTableModel>
#include"parsepe.h"
class IMAGE_FILE_HEADER_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    IMAGE_FILE_HEADER_Model(quint32 addr,byte * data,QObject * parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    int row_count;
    QList<WORD> charList;
    byte * inter_data = Q_NULLPTR;
    quint32 addr = 0;
    QStringList header;
    static const char * charMachine;
    static const char * charNumberOfSections;
    static const char * charTimeDateStamp;
    static const char * charPTST;
    static const char * charNumberOfSymbols;
    static const char * charSizeOfOptionalHeader;
    static const char * charCharacteristics;
    static const char * charCharacter;
};

#endif // IMAGE_FILE_HEADER_MODEL_H
