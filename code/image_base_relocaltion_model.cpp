#include "image_base_relocaltion_model.h"

IMAGE_BASE_RELOCALTION_Model::IMAGE_BASE_RELOCALTION_Model(peFile * pe, QObject * parent)
    :QAbstractTableModel(parent)
{
    header << "peOffset" << "Data" << "重定位数据类型" << "重定位数据地址" << "重定位数据RVA" << "重定位数据RVA=>peOffset";
    pbr = pe->getIBR();
    pefile = pe;
    row_count += (pbr->SizeOfBlock - 8) / 2;

    type << "IMAGE_REL_BASED_ABSOLUTE" << "IMAGE_REL_BASED_HIGHLOW" << "IMAGE_REL_BASED_DIR64";
}


int         IMAGE_BASE_RELOCALTION_Model::columnCount(const QModelIndex &parent) const{
    return header.size();
}
int         IMAGE_BASE_RELOCALTION_Model::rowCount(const QModelIndex &parent) const{
    return row_count;
}
QVariant    IMAGE_BASE_RELOCALTION_Model::data(const QModelIndex &index, int role) const{
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
        if(row < 2){
            return QBrush(QColor(0xff,0xe4,0xc4));
        }
    }
    if(role == Qt::DisplayRole){
    switch (index.column()) {
        case 0:{
            switch (row) {
            case 0:
            case 1:
            {
                sprintf(buffer, "%08X", (byte *)pbr - pefile->getPeBuffer() + row * 4);
                break;
            }
            default:{
                sprintf(buffer, "%08X", (byte *)pbr - pefile->getPeBuffer() + 8 + (row - 2) * 2);
                break;
            }
            }
        break;
        }
        case 1:{
            switch (row) {
            case 0:{
                Util::sprintf_buffer4B(buffer, (byte * )pbr);
                break;
            }
            case 1:{
                Util::sprintf_buffer4B(buffer, ((byte *)pbr) + 4);
                break;
            }
            default:{
                Util::sprintf_buffer2B(buffer, ((byte *)pbr) + 8 + (row - 2) * 2);
            }
            }
        break;
        }
        case 2:{
            switch(row){
            case 0:
            case 1:
                break;
            default:{
                WORD TypeOffset = 0;
                memcpy(&TypeOffset, ((byte *)pbr) + 8 + (row - 2) * 2, 2);
                switch (TypeOffset & 0xf000) {
                case 0:{
                    return type[0];
                }
                case 0x3000:{
                    return type[1];
                }
                case 0xA000:{
                    return type[2];
                }
                }
            }
            break;
            }
            break;
        }
        case 3:{
            switch(row){
            case 0:
            case 1:
                break;
            default:{
                WORD TypeOffset = 0;
                memcpy(&TypeOffset, ((byte *)pbr) + 8 + (row - 2) * 2, 2);
                sprintf(buffer, "%03X", TypeOffset & 0xfff);
                break;
            }
        }
            break;
        }
        case 4:{
            switch (row) {
                case 0:
            case 1:{
                break;
            }
            default:{
                WORD TypeOffset = 0;
                memcpy(&TypeOffset, ((byte *)pbr) + 8 + (row - 2) * 2, 2);
                sprintf(buffer, "%08X", (TypeOffset & 0xfff) + pbr->VirtualAddress);
                break;
            }
            }
        break;
        }
        case 5:{
            switch (row) {
                case 0:
                case 1:
                break;
            default:{
                WORD TypeOffset = 0;
                memcpy(&TypeOffset, ((byte *)pbr) + 8 + (row - 2) * 2, 2);
                TypeOffset &= 0xfff;
                DWORD result = Util::getFileOffsetByRVA(pefile, TypeOffset + pbr->VirtualAddress);
                sprintf(buffer, "%08X", result);
            }
            }
        break;
        }
        case 6:{
            switch (row) {
                case 0:
            case 1:
                break;
            default:{

            }
            }
            break;
        }
        }

        return buffer;
    }
    return QVariant();
}
QVariant    IMAGE_BASE_RELOCALTION_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
