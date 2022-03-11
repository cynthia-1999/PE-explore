#include "binary_ui_model.h"

binary_ui_model::binary_ui_model(byte * data, quint32 base, quint32 size, QObject * parent)
    :QAbstractTableModel(parent)
{
    this->base_address = base;
    this->inter_data = data;
    this->data_size = size;
    header << "peOffset" << "Data" << "Value";
    count = (data_size / 16 + 1) - (data_size % 16 == 0);
}

int binary_ui_model::columnCount(const QModelIndex &parent) const{
    return  3;
}
int binary_ui_model::rowCount(const QModelIndex &parent) const{
    return count;
}
QVariant binary_ui_model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    char buffer[100];
    if(role == Qt::DisplayRole){
        switch (index.column()) {
            case 0:
        {
            sprintf(buffer, "%08X", index.row() * 16 + base_address);
            break;
        }
            case 1:
        {
                int row = index.row();
                if(row + 1 < count || data_size % 16 == 0)
                {
                    quint32 begin = row << 4;
                    sprintf(buffer, "%02X %02X %02X %02X %02X %02X %02X %02X  %02X %02X %02X %02X %02X %02X %02X %02X",
                            inter_data[begin],
                            inter_data[begin + 1],
                            inter_data[begin + 2],
                            inter_data[begin + 3],
                            inter_data[begin + 4],
                            inter_data[begin + 5],
                            inter_data[begin + 6],
                            inter_data[begin + 7],
                            inter_data[begin + 8],
                            inter_data[begin + 9],
                            inter_data[begin + 10],
                            inter_data[begin + 11],
                            inter_data[begin + 12],
                            inter_data[begin + 13],
                            inter_data[begin + 14],
                            inter_data[begin + 15]);
                }
                else
                {
                    int left = data_size % 16;
                    quint32 begin = row << 4;
                    for(int i = 0; i < left; i++)
                    {
                        sprintf(buffer + (i * 3), "%02X ", inter_data[begin + i]);
                    }
                }
                break;
        }
        case 2:
        {
            int row = index.row();
            if(row + 1 < count || data_size % 16 == 0)
            {
                quint32 begin = row << 4;
                for(int i = 0; i < 16; i++){
                    if(inter_data[begin + i] > 0x1f && inter_data[begin + 1] != 0x7f){
                        sprintf(buffer + (i * 3), "%02c ", inter_data[begin + i]);
                    }else{
                        sprintf(buffer + (i * 3), "%02c ", '.');
                    }
                }
            }
            else
            {
                int left = data_size % 16;
                quint32 begin = row << 4;
                for(int i = 0; i < left; i++)
                {
                    if(inter_data[begin + i] > 0x1f && inter_data[begin + 1] != 0x7f){
                        sprintf(buffer + (i * 3), "%02c ", inter_data[begin + i]);
                    }else{
                        sprintf(buffer + (i * 3), "%02c ", '.');
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
QVariant binary_ui_model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
