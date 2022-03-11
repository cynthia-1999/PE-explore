#ifndef IMAGE_DOS_HEADER_MODEL_H
#define IMAGE_DOS_HEADER_MODEL_H
#include<QAbstractTableModel>
#include"headers.h"

class IMAGE_DOS_HEADER_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit IMAGE_DOS_HEADER_Model(byte * data ,QObject * parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    byte * inter_data = Q_NULLPTR;
    QStringList header;

    static const char * char_sig              ;
    static const char * char_Bytes            ;
    static const char * char_page             ;
    static const char * char_relocations      ;
    static const char * char_Size_Of          ;
    static const char * char_Min              ;
    static const char * char_Max              ;
    static const char * char_Intel_SS         ;
    static const char * char_Initial_SP       ;
    static const char * char_imgdos_sig_mz    ;
    static const char * char_CheckSum         ;
    static const char * char_Initial_IP       ;
    static const char * char_Initial_CS       ;
    static const char * char_OTRT             ;
    static const char * char_Overlay_Number   ;
    static const char * char_reserved         ;
    static const char * char_OEM_Identitier   ;
    static const char * char_OEM_information  ;
    static const char * char_OTNH             ;
};

#endif // IMAGE_DOS_HEADER_MODEL_H
