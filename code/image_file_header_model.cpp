#include "image_file_header_model.h"

const char * IMAGE_FILE_HEADER_Model::charMachine = "运行平台";
const char * IMAGE_FILE_HEADER_Model::charNumberOfSections = "节数";
const char * IMAGE_FILE_HEADER_Model::charTimeDateStamp = "文件创建日期";
const char * IMAGE_FILE_HEADER_Model::charPTST = "符号表指针";
const char * IMAGE_FILE_HEADER_Model::charNumberOfSymbols = "符号表中符号个数";
const char * IMAGE_FILE_HEADER_Model::charSizeOfOptionalHeader = "可选头大小";
const char * IMAGE_FILE_HEADER_Model::charCharacteristics = "文件属性";
const char * IMAGE_FILE_HEADER_Model::charCharacter = "属性";
IMAGE_FILE_HEADER_Model::IMAGE_FILE_HEADER_Model(quint32 addr,byte * data, QObject * parent):
    QAbstractTableModel(parent)
{
    this->inter_data = data;
    WORD char_tmp = 0;
    row_count = 7;
    memcpy((void *)&char_tmp, inter_data + 18, 2);
    for(WORD i = 1;i; i <<= 1){
        if((i & char_tmp) != 0){
            row_count++;
            charList.append(i);
        }
    }
    header << "peOffset" << "Data" << "Description" << "Value";
    this->addr = addr;
}

int         IMAGE_FILE_HEADER_Model::columnCount(const QModelIndex &parent) const{
    return 4;
}
int         IMAGE_FILE_HEADER_Model::rowCount(const QModelIndex &parent) const{
    return row_count;
}
QVariant    IMAGE_FILE_HEADER_Model::data(const QModelIndex &index, int role) const{
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
        case 0:
    {
        switch (row) {
        case 0:
        case 1:
            sprintf(buffer, "%08X", addr + row * 2);
            break;
        case 2:
        case 3:
        case 4:
            sprintf(buffer, "%08X", addr + (row - 2) * 4 + 4);
            break;
        case 5:
        case 6:
            sprintf(buffer, "%08X", addr + (row - 5) * 2 + 16);
            break;
        }
        break;
    }
    case 1:
    {
        switch(row){
            case 0:
        {
            sprintf(buffer, "%02X%02X", inter_data[1], inter_data[0]);
            break;
        }
        case 1:
        {
            sprintf(buffer, "%02X%02X", inter_data[3], inter_data[2]);
            break;
        }
        case 2:
        case 3:
        case 4:
        {
            sprintf(buffer, "%02X%02X%02X%02X", (inter_data + 4 + (row - 2) * 4)[3],
                    (inter_data + 4 + (row - 2) * 4)[2],
                    (inter_data + 4 + (row - 2) * 4)[1],
                    (inter_data + 4 + (row - 2) * 4)[0]);
            break;
        }
        case 5:{
            sprintf(buffer, "%02X%02X", (inter_data + 16)[1],(inter_data+16)[0]);
            break;
        }
        case 6:{
            sprintf(buffer, "%02X%02X", (inter_data + 18)[1], (inter_data+18)[0]);
            break;
        }
        default:{
            sprintf(buffer, "%04X", charList.at(row - 7));
            break;
        }
        }
        break;
    }
    case 2:
    {
        switch (row) {
            case 0:
            return charMachine;
        case 1:
            return charNumberOfSections;
        case 2:
            return charTimeDateStamp;
        case 3:
            return charPTST;
        case 4:
            return charNumberOfSymbols;
        case 5:
            return charSizeOfOptionalHeader;
        case 6:
            return charCharacteristics;
        default:
            return charCharacter;
        }
        break;
    }
    case 3:
    {
        switch (row) {
        case 0:{
            WORD machine = 0;
            memcpy((void *)&machine, inter_data, 2);
            return ParsePe::parseMachine(machine);
            }
        case 1:{
            WORD nos = 0;
            memcpy((void *)&nos, inter_data + 2, 2);
            return QString::number(nos);
        }
        case 2:{
            DWORD tmp = 0;
            memcpy((void *)&tmp, inter_data + 4, 4);
            return ParsePe::parseTimeDateStampToString(tmp);
        }
        case 3:{
            return QVariant();
        }
        case 4:{
            DWORD tmp = 0;
            memcpy((void *)&tmp, inter_data + 12, 4);
            return QString::number(tmp);
        }
        case 5:{
                WORD sooh = 0;
                memcpy(&sooh, inter_data + 16, 2);
                return QString::number(sooh);
            }
        case 6:{
            return QVariant();
        }
        default:
            return ParsePe::parseWordToImage_Characteristics(charList.at(row - 7));
        }
        }
    }
        return buffer;
    }
    return QVariant();
}
QVariant    IMAGE_FILE_HEADER_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
