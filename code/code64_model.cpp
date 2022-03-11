#include "code64_model.h"

Code64_Model::Code64_Model(QList<opClass64>* list, QObject * parent)
    :QAbstractTableModel(parent)
{
    this->codeList = list;
    header << "VA";
    header << "Op Code";
    header << "Assembly code";
}

Code64_Model::~Code64_Model(){
    delete codeList;
}

int        Code64_Model::columnCount(const QModelIndex &parent) const{
    return 3;
}
int        Code64_Model::rowCount(const QModelIndex &parent) const{
    return codeList->size();
}
QVariant   Code64_Model::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }
    if(Qt::TextAlignmentRole == role)
    {
        return int(Qt::AlignCenter | Qt::AlignHCenter);
    }
    DWORD row = index.row();
    if(role == Qt::BackgroundRole){
        if(row % 2){
            return QBrush(QColor(0xff,0xfa,0xf0));
        }else{
            return QBrush(QColor(Qt::white));
        }
    }

    if(role == Qt::DisplayRole){
        switch (index.column()) {
        case 0:{
            return QString::number(codeList->at(row).Addr, 16);
            break;
        }
        case 1:{
            return codeList->at(row).opCode;
            break;
        }
        case 2:{
            return codeList->at(row).opInstr;
        }
        }
    }
    return QVariant();
}
QVariant   Code64_Model::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation==Qt::Horizontal&&role==Qt::DisplayRole){
        return header.at(section);
    }else{
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}
