#include "image_thunk_data64_model.h"

IMAGE_THUNK_DATA64_Model::IMAGE_THUNK_DATA64_Model(peFile * pe, quint32 addr, byte * itdArray, QString dllName,QObject * parent)
    :QAbstractTableModel(parent)
{
    this->itdArray = (PIMAGE_THUNK_DATA64)itdArray;
    this->dllName = dllName;
    header << "peOffset" << "Data" << "Description" << "Value";
    this->addr = addr;
    PIMAGE_THUNK_DATA64 tmp = this->itdArray;
    while(tmp->u1.Ordinal){
        row_count++;
        tmp++;
    }
    this->pefile = pe;
}
int         IMAGE_THUNK_DATA64_Model::columnCount(const QModelIndex &parent) const{
    return 4;
}
int         IMAGE_THUNK_DATA64_Model::rowCount(const QModelIndex &parent) const{
    return row_count;
}
QVariant    IMAGE_THUNK_DATA64_Model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    int row = index.row();
    char buffer[100] = {0};
    if(Qt::TextAlignmentRole == role)
    {
        return int(Qt::AlignCenter | Qt::AlignHCenter);
    }
    if(role == Qt::BackgroundRole){
        switch (row) {
        case 0:{
            return QBrush(QColor(0xff,0xfa,0xfa));
        }
        }
    }
    if(role == Qt::DisplayRole){
    switch (index.column()) {
    case 0:{
        switch (row) {
        case 0:{
            return QVariant();
        }
        default:{
            sprintf(buffer, "%08X", addr + (row - 1) * 4);
            break;
        }
        }
        break;
    }
    case 1:{
        switch (row) {
        case 0:{
            return dllName;
        }
        default:{
            DWORD data = itdArray[row - 1].u1.AddressOfData;
            sprintf(buffer, "%08X", data);
            break;
        }
        }
        break;
    }
    case 2:{
        switch (row) {
        case 0:{
            return "dll名称";
        }
        default:{
            return "Hint/Name RVA";
        }
        }
        break;
    }
    case 3:{
        switch (row) {
        case 0:{
            break;
        }
        default:{
            byte *tmp = pefile->getPeBuffer() + Util::getFileOffsetByRVA(pefile,
                                               itdArray[row - 1].u1.Ordinal);
            WORD hint = 0;
            memcpy(&hint, tmp, 2);
            sprintf(buffer, "%04X %s", hint, tmp + 2);
            break;
        }
        }
    }
    }
    return buffer;
    }
    return QVariant();
}
QVariant    IMAGE_THUNK_DATA64_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
