#include "image_dos_signature.h"

Image_Dos_Signature::Image_Dos_Signature(quint32 addr, QObject * parent)
    :QAbstractTableModel(parent)
{
    this->addr = addr;
    header << "peOffset" << "Data" << "Description" <<"Value";
}

int         Image_Dos_Signature::columnCount(const QModelIndex &parent) const{
    return 4;
}
int         Image_Dos_Signature::rowCount(const QModelIndex &parent) const{
    return 1;
}
QVariant    Image_Dos_Signature::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    char buffer[100] = {0};
    if(Qt::TextAlignmentRole == role)
    {
        return int(Qt::AlignCenter | Qt::AlignHCenter);
    }
    if(role == Qt::DisplayRole){
    switch (index.column()) {
        case 0:
            sprintf(buffer, "%08X", addr);
        break;
        case 1:
        sprintf(buffer, "%s", "00004550");
        break;
    case 2:
        sprintf(buffer, "%s", "Signature");
        break;
    case 3:
        sprintf(buffer, "%s", "IMAGE_NT_SIGNATURE PE");
        break;
    }
    return buffer;
    }
    return QVariant();
}

QVariant Image_Dos_Signature::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
