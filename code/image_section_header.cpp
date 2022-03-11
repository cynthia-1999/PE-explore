#include"image_section_header.h"
const char *IMAGE_SECTION_HEADER_Model::charName = "节名称";
const char *IMAGE_SECTION_HEADER_Model::charPV  = "PhysicalAddree/VirtualSize";
const char *IMAGE_SECTION_HEADER_Model::charVA  = "区块RVA";
const char *IMAGE_SECTION_HEADER_Model::charSOD = "对齐后的尺寸";
const char *IMAGE_SECTION_HEADER_Model::charPTRD = "文件中的偏移";
const char *IMAGE_SECTION_HEADER_Model::charPTR = "重定位偏移";
const char *IMAGE_SECTION_HEADER_Model::charPTL = "行号表偏移";
const char *IMAGE_SECTION_HEADER_Model::charNO  = "重定位项数目";
const char *IMAGE_SECTION_HEADER_Model::charNOL = "行号表数目";
const char *IMAGE_SECTION_HEADER_Model::charC   = "区块属性";


IMAGE_SECTION_HEADER_Model::IMAGE_SECTION_HEADER_Model(quint32 addr, byte * data,QObject * parent)
    :QAbstractTableModel(parent)
{
    this->addr = addr;
    this->inter_data = data;
    header << "peOffset" << "Data" << "Description" << "Value";
    DWORD char_tmp = 0;
    memcpy(&char_tmp, inter_data + 36, 4);
    for(DWORD i = 1; i != 0; i <<= 1){
        if((i & char_tmp) != 0){
            charList.append(i);
            if(i == 0x20000000){
                execute = true;
            }
            row_count++;
        }
    }
}

int         IMAGE_SECTION_HEADER_Model::columnCount(const QModelIndex &parent) const{
    return 4;
}
int         IMAGE_SECTION_HEADER_Model::rowCount(const QModelIndex &parent) const{
    return row_count;
}
QVariant    IMAGE_SECTION_HEADER_Model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    int row = index.row();
    char buffer[100] = {0};
    if(Qt::TextAlignmentRole == role)
    {
        return int(Qt::AlignCenter | Qt::AlignHCenter);
    }
    if(role == Qt::DisplayRole){
    switch (index.column()) {
    case 0:{
        switch (row) {
        case 0:{
            sprintf(buffer, "%04X", addr);
            break;
        }
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        {
            sprintf(buffer, "%04X", addr + 8 +(row - 1) * 4);
            break;
        }
        case 7:
        case 8:
        case 9:{
            sprintf(buffer, "%04X", addr + 32 +(row - 7) * 2);
            break;
        }
        default:{
            return QVariant();
        }
        }
        break;
    }
    case 1:{
        switch (row) {
        case 0:{
            for(int i = 0; i < 8; i++){
                if(inter_data[i] == 0){
                    break;
                }
                sprintf(buffer + i * 3, "%02X ", inter_data[i]);
            }
            break;
        }
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        {
            Util::sprintf_buffer4B(buffer, inter_data + 8 + (row - 1) * 4);
            break;
        }
        case 7:
        case 8:
        {
            Util::sprintf_buffer2B(buffer, inter_data + 32 + (row - 7) * 2);
            break;
        }
        case 9:{
            Util::sprintf_buffer4B(buffer, inter_data + 36);
            break;
        }
        default:{
            sprintf(buffer, "%08X", charList.at(row - 10));
        }
        }
        break;
    }
    case 2:{
        switch (row) {
            case 0:
            return charName;
        case 1:
            return charPV;
        case 2:
            return charVA;
        case 3:
            return charSOD;
        case 4:
            return charPTRD;
        case 5:
            return charPTR;
        case 6:
            return charPTL;
        case 7:
            return charNO;
        case 8:
            return charNOL;
        case 9:
            return charC;
        default:
            return "属性";
        }
    }
    case 3:{
        switch (row) {
        case 0:{
            sprintf(buffer, "%s", inter_data);
            break;
        }
        default:{
            if(row > 9){
                return ParsePe::parseDWordToSectionCharacteristics(charList.at(row - 10));
            }
        }
        }
        break;
    }
    }
        return buffer;
    }
    return QVariant();
}
QVariant    IMAGE_SECTION_HEADER_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
