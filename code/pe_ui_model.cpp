#include "pe_ui_model.h"

PE_UI_Model::PE_UI_Model(bool flag, quint64 imagebase, byte * buffer, quint32 size,QObject * parent)
    : QAbstractTableModel(parent)
{
    this->fsize = size;
    this->inter_data = buffer;
    this->image_base = imagebase;
    this->pe32Flag = flag;
    count = (fsize / 16 + 1) - (fsize % 16 == 0);
    init();
}


int PE_UI_Model::columnCount(const QModelIndex &parent) const{
    //固定3列
    return 3;
}

QVariant PE_UI_Model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    char buffer[100];
    if(role == Qt::DisplayRole){
        switch (index.column()) {
            case 0:
        {
            if(address_op == 0 || address_op == 1)
                if(pe32Flag)
                    sprintf(buffer, "%08X", index.row() * 16);
                else
                    sprintf(buffer, "%016X", index.row() * 16);
            else
                if(pe32Flag)
                    sprintf(buffer, "%08X", index.row() * 16 + image_base);
                else
                    sprintf(buffer, "%016X", (quint64)index.row() * 16 + image_base);
            break;
        }
            case 1:
        {
                int row = index.row();
                if(row + 1 < count)
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
                    int left = fsize % 16;
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
            if(row + 1 < count)
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
                int left = fsize % 16;
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
                break;
        }
        }
        return buffer;
    }
    return QVariant();
}

int PE_UI_Model::rowCount(const QModelIndex &parent) const{
    return count;
}

QVariant PE_UI_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
void PE_UI_Model::init(){
    header << "peOffset" << "Data" << "Value";
}

void PE_UI_Model::set_addressOp(int op){
    address_op = op;
    switch(op){
        case 0:
        header[0] = "peOffset";
        break;
    case 1:
        header[0] = "RVA";
        break;
    case 2:
        header[0] = "VA";
        break;
    }
}
