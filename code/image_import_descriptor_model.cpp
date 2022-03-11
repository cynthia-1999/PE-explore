#include "image_import_descriptor_model.h"


const char * IMAGE_IMPORT_DESCRIPTION_Model::charCO               ="Characteristics/OriginalFirstThunk"     ;
const char * IMAGE_IMPORT_DESCRIPTION_Model::charTimeDateStamp    ="TimeDateStamp"    ;
const char * IMAGE_IMPORT_DESCRIPTION_Model::charFor              ="ForwarderChain"     ;
const char * IMAGE_IMPORT_DESCRIPTION_Model::charName             ="Name"     ;
const char * IMAGE_IMPORT_DESCRIPTION_Model::charFirstThunk       ="FirstThunk"     ;
IMAGE_IMPORT_DESCRIPTION_Model::IMAGE_IMPORT_DESCRIPTION_Model(peFile * p,QObject *parent)
    :QAbstractTableModel(parent)
{
    this->pefile = p;
    PIMAGE_IMPORT_DESCRIPTOR tmp = p->getIIDArray();
    while(tmp->Name != 0){
        tmp++;
        row_count++;
    }
    row_count *= 5;
    offset = (byte *)p->getIIDArray() - p->getPeBuffer();
    header << "peOffset" << "Data" << "Description" << "Value";
}

int         IMAGE_IMPORT_DESCRIPTION_Model::columnCount(const QModelIndex &parent) const{
    return 4;
}
int         IMAGE_IMPORT_DESCRIPTION_Model::rowCount(const QModelIndex &parent) const{
    return row_count;
}
QVariant    IMAGE_IMPORT_DESCRIPTION_Model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }

    int row = index.row();

    if(role == Qt::BackgroundRole){
        if((row / 5) % 2){
            return QBrush(QColor(Qt::lightGray));
        }else{
            return QBrush(QColor(Qt::white));
        }
    }

    char buffer[100] = {0};
    if(Qt::TextAlignmentRole == role)
    {
        return int(Qt::AlignCenter | Qt::AlignHCenter);
    }
    if(role == Qt::DisplayRole){
    PIMAGE_IMPORT_DESCRIPTOR iidArray = pefile->getIIDArray();
    switch (index.column()) {
    case 0:{
        sprintf(buffer, "%08X", row * 4 + offset);
        break;
    }
    case 1:{
        Util::sprintf_buffer4B(buffer, (byte *)iidArray + row * 4);
        break;
    }
    case 2:{
        switch (row % 5) {
            case 0: return charCO;
        case 1: return charTimeDateStamp;
        case 2: return charFor;
        case 3: return charName;
        case 4: return charFirstThunk;
        }
    }
    case 3:{
        switch (row % 5) {
        case 1:{
            DWORD time = 0;
            memcpy(&time, &(iidArray[row / 5].TimeDateStamp), 4);
            if(time == 0){
                break;
            }else{
                return ParsePe::parseTimeDateStampToString(time);
            }
        }
        case 3: {
            DWORD nameRva = 0;
            memcpy(&nameRva, &(iidArray[row / 5].Name), 4);
            DWORD namePeOffset = Util::getFileOffsetByRVA(pefile, nameRva);
            sprintf(buffer, "%s", pefile->getPeBuffer() + namePeOffset);
            break;
        }
        }
    }
    }
        return buffer;
    }
    return QVariant();
}
QVariant    IMAGE_IMPORT_DESCRIPTION_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
