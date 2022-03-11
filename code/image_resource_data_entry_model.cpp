#include "image_resource_data_entry_model.h"

IMAGE_RESOURCE_Data_Entry_Model::IMAGE_RESOURCE_Data_Entry_Model(quint32 base_addr, peFile * pe,QObject * parent)
    :QAbstractTableModel(parent)
{
    this->base_addr = base_addr;
    this->pefile = pe;
    header << "peOffset" << "Data" << "Description";
}

int         IMAGE_RESOURCE_Data_Entry_Model::columnCount(const QModelIndex &parent) const{
    return 3;
}
int         IMAGE_RESOURCE_Data_Entry_Model::rowCount(const QModelIndex &parent) const{
    return 4;
}
QVariant    IMAGE_RESOURCE_Data_Entry_Model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    char buffer[100] = {0};
    int row = index.row();
    if(Qt::TextAlignmentRole == role)
    {
        return int(Qt::AlignCenter | Qt::AlignHCenter);
    }
    if(Qt::DisplayRole == role){
        switch (index.column()) {
        case 0:{
            sprintf(buffer, "%08X", base_addr + row * 4);
            break;
        }
        case 1:{
            Util::sprintf_buffer4B(buffer, pefile->getPeBuffer() + base_addr + row * 4);
            break;
        }
        case 2:{
            switch (row) {
                case 0:
                return "资源数据的RVA";
            case 1:
                return "资源数据的长度";
            case 2:
                return "代码页 一般为0";
            case 3:
                return "保留字段";
            }
            break;
        }
        }
        return buffer;
    }

    return QVariant();
}
QVariant    IMAGE_RESOURCE_Data_Entry_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
