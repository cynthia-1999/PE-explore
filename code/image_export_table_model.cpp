#include "image_export_table_model.h"

IMAGE_EXPORT_TABLE_Model::IMAGE_EXPORT_TABLE_Model(peFile * pe ,QObject * parent)
    : QAbstractTableModel(parent)
{
    this->pefile = pe;
    header << "peOffset" << "Data" << "Description" << "Value";
    this->IEDArray = pefile->getIEDArray();

    row_count += IEDArray[0].NumberOfNames;
}

int         IMAGE_EXPORT_TABLE_Model::columnCount(const QModelIndex &parent) const{
    return 4;
}
int         IMAGE_EXPORT_TABLE_Model::rowCount(const QModelIndex &parent) const{
    return row_count;
}
QVariant    IMAGE_EXPORT_TABLE_Model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    char buffer[100] = {0};
    if(Qt::TextAlignmentRole == role)
    {
        return int(Qt::AlignCenter | Qt::AlignHCenter);
    }
    int row = index.row();
    if(role == Qt::BackgroundRole){
        if(row <= 10){
            return QBrush(QColor(0xff,0xe4,0xc4));
        }
    }
    quint32 iedAddr = (byte *)pefile->getIEDArray() - pefile->getPeBuffer();
    quint32 eatAddr = Util::getFileOffsetByRVA(pefile, IEDArray[0].AddressOfNames);
    quint32 eotAddr = Util::getFileOffsetByRVA(pefile, IEDArray[0].AddressOfNameOrdinals);
    if(role == Qt::DisplayRole){
    switch (index.column()) {
    case 0:{
        switch (row) {
        case 0:{
            sprintf(buffer, "%08X", iedAddr);
            break;
        }
        case 1:{
            sprintf(buffer, "%08X", iedAddr + 4);
            break;
        }
        case 2:{
            sprintf(buffer, "%08X", iedAddr + 8);
            break;
        }
        case 3:{
            sprintf(buffer, "%08X", iedAddr + 10);
            break;
        }
        case 4:{
            sprintf(buffer, "%08X", iedAddr + 12);
            break;
        }
        case 5:{
            sprintf(buffer, "%08X", iedAddr + 16);
            break;
        }
        case 6:{
            sprintf(buffer, "%08X", iedAddr + 20);
            break;
        }
        case 7:{
            sprintf(buffer, "%08X", iedAddr + 24);
            break;
        }
        case 8:{
            sprintf(buffer, "%08X", iedAddr + 28);
            break;
        }
        case 9:{
            sprintf(buffer, "%08X", iedAddr + 32);
            break;
        }
        case 10:{
            sprintf(buffer, "%08X", iedAddr + 36);
            break;
        }default:{
            sprintf(buffer, "%08X", eatAddr + (row - 11) * 4);
            break;
        }
        }
        break;
    }
    case 1:{
        switch (row) {
        case 0:
        case 1:{
            Util::sprintf_buffer4B(buffer, row * 4 + pefile->getPeBuffer() + iedAddr);
            break;
        }
        case 2:
        case 3:{
            Util::sprintf_buffer2B(buffer, (row - 2)*2 + pefile->getPeBuffer() + iedAddr + 8);
            break;
        }
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:{
            Util::sprintf_buffer4B(buffer, (row - 4) * 4 + pefile->getPeBuffer() + iedAddr + 12);
            break;
        }default:{
            Util::sprintf_buffer4B(buffer, pefile->getPeBuffer() + eatAddr + (row - 11) * 4);
            break;
        }
        }
        break;
    }
    case 2:{
        switch(row){
        case 0:
            return "Characteristics 总是为0";
        case 1:
            return "文件生成时间";
        case 2:
            return "主版本号";
        case 3:
            return "次版本号";
        case 4:
            return "模块名称";
        case 5:
            return "序号基数";
        case 6:
            return "EAT中的条目数量";
        case 7:
            return "ENT中的条目数量";
        case 8:
            return "EAT的RVA数组";
        case 9:
            return "ENT的RVA数组";
        case 10:
            return "输出序数表的RVA";
        }
    default:{
            return "导出序号/导出名称";
        }
        break;
    }
    case 3:{
        switch(row){
        case 1:{
            DWORD time = 0;
            memcpy(&time, (byte *)pefile->getIEDArray(), 4);
            return ParsePe::parseTimeDateStampToString(time);
        }
        case 4:{
            return QString(
                        (char *)(Util::getFileOffsetByRVA(pefile, IEDArray[0].Name) + pefile->getPeBuffer()
                        ));
            break;
        }
        default:{
            if(row > 10){
            quint32 rva_name = 0;
            memcpy(&rva_name, pefile->getPeBuffer() + eatAddr + (row - 11) * 4, 4);
            quint32 nameOffset = Util::getFileOffsetByRVA(pefile, rva_name);
            WORD ord = 0;
            memcpy(&ord, pefile->getPeBuffer() + eotAddr + (row - 11) * 2, 2);
            sprintf(buffer, "%04d  %s", ord, (char *)(pefile->getPeBuffer() + nameOffset));
            break;
            }
            break;
        }
        }

        break;
    }
    }
    return buffer;
    }
    return QVariant();
}
QVariant    IMAGE_EXPORT_TABLE_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
