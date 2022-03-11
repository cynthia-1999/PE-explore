#ifndef IMAGE_OPTIONSAL_HEADER_MODEL_H
#define IMAGE_OPTIONSAL_HEADER_MODEL_H
#include<QAbstractTableModel>
#include"headers.h"
#include"util.h"
#include"parsepe.h"
class IMAGE_OPTIONAL_HEADER_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    IMAGE_OPTIONAL_HEADER_Model(quint32 addr,byte * data,bool pe32Flag,QObject * parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    quint32 addr;
    byte * inter_data;
    bool pe32Flag = true;
    QStringList header;

    static const char * charMagic                            ;
    static const char * charMajor                            ;
    static const char * charMinor                            ;
    static const char * charSizeOfCode                       ;
    static const char * charSizeOfInitializedData            ;
    static const char * charSizeOfUnitializedData            ;
    static const char * charAddressOfEntryPoint              ;
    static const char * charBaseOfCode                       ;
    static const char * charBaseOfData                       ;
    static const char * charImageBase                        ;
    static const char * charSectionAlignment                 ;
    static const char * charFileAlignment                    ;
    static const char * charMajorOperatingSystemVersion      ;
    static const char * charMinorOperatingSystemVersion      ;
    static const char * charMajorImageVersion                ;
    static const char * charMinorImageVersion                ;
    static const char * charMajorSubSystemVersion            ;
    static const char * charMionrSubSystemVersion            ;
    static const char * charWin32VersionValue                ;
    static const char * charSizeOfImage                      ;
    static const char * charSizeOfHeaders                    ;
    static const char * charCheckSum                         ;
    static const char * charSubSystem                        ;
    static const char * charDllCharacteristics               ;
    static const char * charSizeOfStackReserve               ;
    static const char * charSizeOfStackCommit                ;
    static const char * charSizeOfHeapReserve                ;
    static const char * charSizeOfHeapCommit                 ;
    static const char * charLoaderFlags                      ;
    static const char * charNumberOfRvaAndSizes              ;
    static const char * charDirectory_ET                     ;
    static const char * charDirectory_IT                     ;
    static const char * charDirectory_RT                     ;
    static const char * charDirectory_Exception_Table        ;
    static const char * charDirectory_ST                     ;
    static const char * charDirectory_BRT                    ;
    static const char * charDirectory_D                      ;
    static const char * charDirectory_C                      ;
    static const char * charDirectory_GP                     ;
    static const char * charDirectory_TLS                    ;
    static const char * charDirectory_LC                     ;
    static const char * charDirectory_BI                     ;
    static const char * charDirectory_IAT                    ;
    static const char * charDirectory_DI                     ;
    static const char * charDirectory_CD                     ;
    static const char * charDirectory_Reserve                ;
};

#endif // IMAGE_OPTIONSAL_HEADER_MODEL_H
